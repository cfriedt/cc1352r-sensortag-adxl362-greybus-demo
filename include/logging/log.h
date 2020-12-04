#ifndef LOGGING_LOG_H_
#define LOGGING_LOG_H_

#include <stdint.h>
#include <stdio.h>

#ifdef DEBUG
#define LOG_DBG(fmt, args...) fprintf(stdout, "D: %s.%s: %d: " fmt "\n", __log_label, __func__, __LINE__, ##args)
#else
#define LOG_DBG(fmt, args...)
#endif
#define LOG_INF(fmt, args...) fprintf(stdout, "I: %s.%s: %d: " fmt "\n", __log_label, __func__, __LINE__, ##args)
#define LOG_WRN(fmt, args...) fprintf(stderr, "W: %s.%s: %d: " fmt "\n", __log_label, __func__, __LINE__, ##args)
#define LOG_ERR(fmt, args...) fprintf(stderr, "E: %s.%s: %d: " fmt "\n", __log_label, __func__, __LINE__, ##args)

#ifdef DEBUG
#define LOG_HEXDUMP_DBG(data, length, prefix) log_hexdump(__log_label, __func__, __LINE__, 'D', data, length, prefix)
#else
#define LOG_HEXDUMP_DBG(data, length, prefix)
#endif
#define LOG_HEXDUMP_INF(data, length, prefix) log_hexdump(__log_label, __func__, __LINE__, 'I', data, length, prefix)
#define LOG_HEXDUMP_WRN(data, length, prefix) log_hexdump(__log_label, __func__, __LINE__, 'W', data, length, prefix)
#define LOG_HEXDUMP_ERR(data, length, prefix) log_hexdump(__log_label, __func__, __LINE__, 'E', data, length, prefix)

#define LOG_MODULE_REGISTER(label, level) \
	static const char *__log_label = #label

void log_hexdump(const char *label, const char *func, int line, char type, uint8_t *data, size_t length, const char *prefix);

#endif
