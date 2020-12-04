#ifndef SYS_BYTEORDER_H_
#define SYS_BYTEORDER_H_

#define __USE_MISC 1
#include <endian.h>

#include <zephyr/types.h>

#define sys_le16_to_cpu(x) le16toh(x)
#define sys_le32_to_cpu(x) le32toh(x)
#define sys_le64_to_cpu(x) le64toh(x)

#endif
