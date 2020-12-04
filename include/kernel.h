#ifndef KERNEL_H_
#define KERNEL_H_

#include <errno.h>
#include <unistd.h>

#include "config.h"
#include <sys/util.h>
#include <zephyr/types.h>

#define K_MSEC(x) (x*1000)

static inline int k_sleep(uint32_t us) {
	extern int usleep(uint32_t usec);
	usleep(us);
	return 0;
}

#endif
