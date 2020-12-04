#ifndef DEVICE_H_
#define DEVICE_H_

#include <devicetree.h>

struct device {
        void *data;
	const void *config;
	const void *api;
};

const struct device *device_get_binding(const char *name);

#endif
