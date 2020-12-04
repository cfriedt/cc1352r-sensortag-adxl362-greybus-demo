#ifndef DRIVERS_SENSOR_H_
#define DRIVERS_SENSOR_H_

#include <stdbool.h>
#include <stdint.h>
#include <device.h>

#define SENSOR_G              9806650LL

enum sensor_attribute {
	SENSOR_ATTR_FULL_SCALE,
	SENSOR_ATTR_LOWER_THRESH,
	SENSOR_ATTR_SAMPLING_FREQUENCY,
	SENSOR_ATTR_UPPER_THRESH,
};

enum sensor_channel {
	SENSOR_CHAN_DIE_TEMP,
	SENSOR_CHAN_ACCEL_X,
	SENSOR_CHAN_ACCEL_Y,
	SENSOR_CHAN_ACCEL_Z,
	SENSOR_CHAN_ACCEL_XYZ,
	SENSOR_CHAN_ALL,
};

struct sensor_value {
	int32_t val1;
	int32_t val2;
};

struct sensor_driver_api {
	int (*attr_set_thresh)(const struct device *dev,
                                   enum sensor_channel chan,
                                   enum sensor_attribute attr,
                                   const struct sensor_value *val);
	int (*attr_set)(const struct device *dev,
                            enum sensor_channel chan,
                            enum sensor_attribute attr,
                            const struct sensor_value *val);
	int (*sample_fetch)(const struct device *dev,
                                enum sensor_channel chan);
	int (*channel_get)(const struct device *dev,
                               enum sensor_channel chan,
                               struct sensor_value *val);
};

static inline double sensor_value_to_double(struct sensor_value *val)
{
	return (double)val->val1 + (double)val->val2 / 1000000;
}

static inline int32_t sensor_ms2_to_g(const struct sensor_value *ms2)
{
        int64_t micro_ms2 = ms2->val1 * 1000000LL + ms2->val2;

        if (micro_ms2 > 0) {
                return (micro_ms2 + SENSOR_G / 2) / SENSOR_G;
        } else {
                return (micro_ms2 - SENSOR_G / 2) / SENSOR_G;
        }
}

#endif

