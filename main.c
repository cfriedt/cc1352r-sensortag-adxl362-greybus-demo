#include <stdio.h>
#include <unistd.h>
extern int usleep(int);

#include <drivers/sensor.h>
#include <logging/log.h>

#include "adxl362.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

extern int adxl362_init(const struct device *dev);
extern const struct sensor_driver_api *get_adxl362_api(void);
extern void *get_adxl362_data(void);
extern void *get_adxl362_config(void);

int main(int argc, char *argv[]) {
	int r;
	const struct sensor_driver_api *api = get_adxl362_api();
	struct adxl362_config *config = (struct adxl362_config *)get_adxl362_config();
	struct adxl362_data *data = get_adxl362_data();
	struct device *dev = (struct device *) device_get_binding("foo");
	struct sensor_value val[4];
	double d;
	dev->api = api;
	dev->data = data;
	dev->config = config;
	config->spi_name = "/dev/spidev0.0";
	config->spi_max_frequency = 6000000;

	r = adxl362_init(dev);
	if (r != 0) {
		LOG_ERR("adxl362_init() failed (%d)", r);
		goto out;
	}

	for(;;) {
		api->sample_fetch(dev, SENSOR_CHAN_ALL);
		api->channel_get(dev, SENSOR_CHAN_ACCEL_X, &val[0]);
		api->channel_get(dev, SENSOR_CHAN_ACCEL_Y, &val[1]);
		api->channel_get(dev, SENSOR_CHAN_ACCEL_Z, &val[2]);
		api->channel_get(dev, SENSOR_CHAN_DIE_TEMP, &val[3]);
		printf("(x,y,z,%cC) =  %10.6f, %10.6f, %10.6f, %10.6f\n",
			176,
			sensor_value_to_double(&val[0]),
			sensor_value_to_double(&val[1]),
			sensor_value_to_double(&val[2]),
			sensor_value_to_double(&val[3]));
	}

out:
	return 0;
}
