#include <ctype.h>
#include <stdio.h>
#include <stdint.h>

#include <device.h>
#include <sys/util.h>

static struct device dummy;

const struct device *device_get_binding(const char *name) {
	(void) name;
	return &dummy;
}

static void hexdump_line(FILE *op, const uint8_t *data, size_t length) {
	fprintf(op, "%p | ", data);
	for(size_t i = 0, N = MIN(16, length); i < N; ++i) {
		if (i == 8) {
			fprintf(op, " ");
		}
		fprintf(op, "%02x", data[i]);
	}

	for(size_t i = 0; i < 16 - length; ++i) {
		fprintf(op, "..");
	}

	if (length >= 8) {
		fprintf(op, "..");
	}

	fprintf(op, " | ");
	for(size_t i = 0, N = MIN(16, length); i < N; ++i) {
		int c = data[i];
		fprintf(op, "%c", isprint(c) ? c : '.');
	}

	fprintf(op, "\n");
}

void log_hexdump(const char *label, const char *func, int line, char type, const uint8_t *data, size_t length, const char *prefix)
{
	FILE *op;
	switch(type) {
	case 'D':
	case 'I':
		op = stdout;
		break;
	case 'W':
	case 'E':
		op = stderr;
		break;
	}

	fprintf(op, "%c: %s.%s: %d: %s\n", type, label, func, line, prefix);
	for(size_t chunk = MIN(16, length); length > 0; data += chunk, length -= chunk, chunk = MIN(16, length)) {
		hexdump_line(op, data, length);
	}
}

