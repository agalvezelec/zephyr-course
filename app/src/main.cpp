#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

int main(void) {
    const struct device *my_sensor = DEVICE_DT_GET_ANY(our_driver);

    if (!device_is_ready(my_sensor)) {
        return -1;
    }

    while (1) {
        /* Call fetch -> Driver will set LED ON */
        sensor_sample_fetch(my_sensor);
        k_msleep(1000);
        
        /* Call get -> Driver will set LED OFF */
        struct sensor_value dummy_val;
        sensor_channel_get(my_sensor, SENSOR_CHAN_ALL, &dummy_val);
        k_msleep(1000);
    }
}
