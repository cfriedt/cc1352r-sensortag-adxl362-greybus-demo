#ifndef SYS_UTIL_H_
#define SYS_UTIL_H_

#define ARRAY_SIZE(x) (int)(sizeof(x)/sizeof((x)[0]))
#define BIT(n) (1ULL << (n))
#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#endif
