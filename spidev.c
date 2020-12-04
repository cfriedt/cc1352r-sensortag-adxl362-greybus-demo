#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <drivers/spi.h>
#include <logging/log.h>

#include "adxl362.h"

LOG_MODULE_REGISTER(spidev, CONFIG_SPIDEV_LOG_LEVEL);

#ifdef TEST
//#if 1
static uint8_t regs[256] = {
	[ADXL362_REG_PARTID] = 0362,
};

/*
 * There can be "active" registers, as long as the read / write operations
 * are behind a function call
 */
static void read_reg(uint8_t *dst, uint8_t addr, uint8_t len)
{
	for ( ; len > 0; ++addr, ++dst, --len) {
		switch(addr) {
		case ADXL362_REG_PARTID:
			*dst = 0362;
			break;
		default:
			*dst = regs[addr];
			break;
		}
	}
}

static void onReset() {
	LOG_DBG("Reset..");
}

static void write_reg(uint8_t addr, uint8_t *src, uint8_t len)
{
	for ( ; len > 0; ++addr, ++src, --len) {
		switch (addr) {
		case ADXL362_REG_SOFT_RESET:
			if (addr == ADXL362_REG_SOFT_RESET && *src == 0x52) {
				onReset();
			}
			break;
		default:
			regs[addr] = *src;
			break;
		}
	}
}

int spi_transceive(const struct device *dev, const struct spi_config *cfg, const struct spi_buf_set *tx_bufs, const struct spi_buf_set *rx_bufs)
{
	int r = 0;
	uint8_t op;
	uint8_t addr;
	const struct spi_buf *rx;
	const struct spi_buf *tx = &tx_bufs->buffers[0];
	const struct adxl362_config *adxl362_config = (const struct adxl362_config *)dev->config;

	op = tx->buf[0];
	switch(op) {
	case ADXL362_WRITE_REG:
		LOG_HEXDUMP_DBG(tx->buf, tx->len, "TX 0");
		addr = tx->buf[1];
		tx = &tx_bufs->buffers[1];

		// perform I/O
		write_reg(addr, tx->buf, tx->len);

		LOG_HEXDUMP_DBG(tx->buf, tx->len, "TX 1");
		break;

	case ADXL362_READ_REG:
		LOG_HEXDUMP_DBG(tx->buf, tx->len, "TX 0");
		addr = tx->buf[1];
		rx = &rx_bufs->buffers[1];

		// perform I/O
		read_reg(rx->buf, addr, rx->len);

		LOG_HEXDUMP_DBG(rx->buf, rx->len, "RX 1");
		break;

	default:
		LOG_DBG("unhandled op %02x", op);
		return -EIO;
	}

	return r;
}
#else
int spi_transceive(const struct device *dev, const struct spi_config *cfg, const struct spi_buf_set *tx_bufs, const struct spi_buf_set *rx_bufs)
{
	int r;
	int fd;
	uint8_t op;
	const struct spi_buf *rx;
	struct spi_ioc_transfer	xfer[2] = {};
	const struct spi_buf *tx = &tx_bufs->buffers[0];
	const struct adxl362_config *adxl362_config = (const struct adxl362_config *)dev->config;

	uint8_t rxbuf[3];

	r = open(adxl362_config->spi_name, O_RDWR);
	if (-1 == r) {
		LOG_ERR("open: %s (%d)", strerror(errno), errno);
		return -errno;
	}

	fd = r;

	//LOG_DBG("cfg: frequency: %u operation: %u slave: %u", cfg->frequency, cfg->operation, cfg->slave);

	op = tx->buf[0];
	switch(op) {
	case ADXL362_WRITE_REG:
		//LOG_HEXDUMP_DBG(tx->buf, tx->len, "TX 0");
		tx = &tx_bufs->buffers[1];
		//LOG_HEXDUMP_DBG(tx->buf, tx->len, "TX 1");

		// perform I/O
		xfer[0].speed_hz = cfg->frequency;
		xfer[0].bits_per_word = SPI_WORD_SIZE_GET(cfg->operation);
		xfer[0].tx_buf = (uint64_t)tx_bufs->buffers[0].buf;
		xfer[0].len = (uint64_t)tx_bufs->buffers[0].len;

//		LOG_DBG("xfer[0]: speed_hz: %u bits_per_word: %u tx_buf: %p len: %u",
//			xfer[0].speed_hz, xfer[0].bits_per_word, (void *)xfer[0].tx_buf, xfer[0].len);

		xfer[1].speed_hz = cfg->frequency;
		xfer[1].bits_per_word = SPI_WORD_SIZE_GET(cfg->operation);
		xfer[1].tx_buf = (uint64_t)tx_bufs->buffers[1].buf;
		xfer[1].len = (uint64_t)tx_bufs->buffers[1].len;

//		LOG_DBG("xfer[1]: speed_hz: %u bits_per_word: %u tx_buf: %p len: %u",
//			xfer[1].speed_hz, xfer[1].bits_per_word, (void *)xfer[1].tx_buf, xfer[1].len);

		r = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
		if (-1 == r) {
			LOG_ERR("ioctl: %s (%d)", strerror(errno), errno);
			r = -errno;
			goto close_fd;
		}

		break;

	case ADXL362_READ_REG:
		//LOG_HEXDUMP_DBG(tx->buf, tx->len, "TX 0");
		rx = &rx_bufs->buffers[1];

		// perform I/O
		xfer[0].speed_hz = cfg->frequency;
		xfer[0].bits_per_word = SPI_WORD_SIZE_GET(cfg->operation);
		xfer[0].tx_buf = (uint64_t)tx->buf;
		xfer[0].rx_buf = (uint64_t)tx->buf;
		xfer[0].len = tx->len;

//		LOG_DBG("xfer[0]: speed_hz: %u bits_per_word: %u tx_buf: %p len: %u",
//			xfer[0].speed_hz, xfer[0].bits_per_word, (void *)xfer[0].tx_buf, xfer[0].len);

		xfer[1].speed_hz = cfg->frequency;
		xfer[1].bits_per_word = SPI_WORD_SIZE_GET(cfg->operation);
		xfer[1].rx_buf = (uint64_t)rx->buf;
		xfer[1].len = rx->len;

//		LOG_DBG("xfer[1]: speed_hz: %u bits_per_word: %u rx_buf: %p len: %u",
//			xfer[1].speed_hz, xfer[1].bits_per_word, (void *)xfer[1].rx_buf, xfer[1].len);

		r = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
		if (-1 == r) {
			LOG_ERR("ioctl: %s (%d)", strerror(errno), errno);
			r = -errno;
			goto close_fd;
		}

//		LOG_HEXDUMP_DBG(rx->buf, rx->len, "RX 1");
		break;

	default:
		LOG_DBG("unhandled op %02x", op);
		return -EIO;
	}

	r = 0;

close_fd:
	close(fd);

	return r;
}
#endif
