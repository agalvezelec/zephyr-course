#define DT_DRV_COMPAT our_driver

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);
// This was a bug from the video
//LOG_MODULE_DECLARE(our_driver, LOG_LEVEL_INF);

// Use DTS alias to control the LED
#define LED_NODE DT_ALIAS(app_led)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);


// Dynamic struct (only for the compiler)
struct our_driver_data {
    uint8_t blink_mode;
};

// Custom API
// Args: device struct pointer, blink mode integer from main.cpp)
void our_driver_custom_api_fn(struct device *dev, uint8_t new_mode){
	struct our_driver_data *data = (struct our_driver_data *)dev->data;
	data->blink_mode= new_mode;
	LOG_INF("CUSTOM API: Blink mode modified. blink_mode: %d", new_mode);

}


// Fetch function: activate LED
static int our_driver_sample_fetch(const struct device *dev, enum sensor_channel chan) {
    LOG_INF("sensor_sample_fetch called: LED ON");
    return gpio_pin_set_dt(&led, 1); //GPIO standard function
}


// GET function: deactivate LED
static int our_driver_channel_get(const struct device *dev, 
                                  enum sensor_channel chan, 
                                  struct sensor_value *val) {
    LOG_INF("sensor_channel_get called: LED off");
    return gpio_pin_set_dt(&led, 0);
}

// GPIO init
static int our_driver_init(const struct device *dev){
	if(!gpio_is_ready_dt(&led)){
		LOG_ERR("GPIO pin not ready");
		return -ENODEV;
	}
	// Default init
	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    
    	LOG_INF("Driver: our_driver initialization completed."); 
	return 0;

}

// Bind functions to API
static const struct sensor_driver_api our_driver_api_funcs = {
    .sample_fetch = our_driver_sample_fetch,
    .channel_get = our_driver_channel_get,
};


// Instance of dynamic data struct (RAM data)
static struct our_driver_data my_driver_data;



DEVICE_DT_INST_DEFINE(0, our_driver_init, NULL, &my_driver_data, NULL, POST_KERNEL, 80, &our_driver_api_funcs);
