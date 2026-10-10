#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

extern "C" {
    void our_driver_custom_api_fn(const struct device *dev, uint8_t new_mode);
}



//SHELL
const struct device *my_sensor = DEVICE_DT_GET_ANY(our_driver);

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv) {
    sensor_sample_fetch(my_sensor);
    shell_print(sh, "Fetch executed: LED ON");
    return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv) {
    struct sensor_value dummy_val;
    sensor_channel_get(my_sensor, SENSOR_CHAN_ALL, &dummy_val);
    shell_print(sh, "Read executed: LED OFF. Value: %d.%06d", dummy_val.val1, dummy_val.val2);
    return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv) {
    bool ready = device_is_ready(my_sensor);
    shell_print(sh, "Device Name: %s", my_sensor->name);
    shell_print(sh, "Ready State: %s", ready ? "Yes" : "No");
    return 0;
}

//Subcomands array for root 'sensor'
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor_cmds,
    SHELL_CMD(fetch, NULL, "Calls sensor_sample_fetch()", cmd_sensor_fetch),
    SHELL_CMD(info, NULL, "Prints the device name and ready state", cmd_sensor_info),
    SHELL_CMD(read, NULL, "Calls sensor_channel_get() and prints result", cmd_sensor_read),
    SHELL_SUBCMD_SET_END
); 


SHELL_CMD_REGISTER(sensor, &sub_sensor_cmds, "Sensor root shell command", NULL); 




int main(void) {
    const struct device *my_sensor = DEVICE_DT_GET_ANY(our_driver);

    if (!device_is_ready(my_sensor)) {
        return -1;
    }

	uint8_t mode_counter = 0;

    while (1) {

	our_driver_custom_api_fn(my_sensor, mode_counter++);

        /* Call fetch -> Driver will set LED ON */
        sensor_sample_fetch(my_sensor);
        k_msleep(1000);
        
        /* Call get -> Driver will set LED OFF */
        struct sensor_value dummy_val;
        sensor_channel_get(my_sensor, SENSOR_CHAN_ALL, &dummy_val);
        k_msleep(1000);
    }
}
