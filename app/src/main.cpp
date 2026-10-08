#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

extern "C" {
    void our_driver_custom_api_fn(const struct device *dev, uint8_t new_mode);
}

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
