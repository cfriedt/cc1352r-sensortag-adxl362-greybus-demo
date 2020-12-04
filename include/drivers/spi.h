#ifndef DRIVERS_SPI_H_
#define DRIVERS_SPI_H_

#include <device.h>
#include <sys/util.h>
#include <zephyr/types.h>

/**
 * @brief SPI operational mode
 */
#define SPI_OP_MODE_MASTER      0
#define SPI_OP_MODE_SLAVE       BIT(0)
#define SPI_OP_MODE_MASK        0x1
#define SPI_OP_MODE_GET(_operation_) ((_operation_) & SPI_OP_MODE_MASK)

/**
 * @brief SPI Polarity & Phase Modes
 */

/**
 * Clock Polarity: if set, clock idle state will be 1
 * and active state will be 0. If untouched, the inverse will be true
 * which is the default.
 */
#define SPI_MODE_CPOL           BIT(1)

/**
 * Clock Phase: this dictates when is the data captured, and depends
 * clock's polarity. When SPI_MODE_CPOL is set and this bit as well,
 * capture will occur on low to high transition and high to low if
 * this bit is not set (default). This is fully reversed if CPOL is
 * not set.
 */
#define SPI_MODE_CPHA           BIT(2)

/**
 * Whatever data is transmitted is looped-back to the receiving buffer of
 * the controller. This is fully controller dependent as some may not
 * support this, and can be used for testing purposes only.
 */
#define SPI_MODE_LOOP           BIT(3)

#define SPI_MODE_MASK           (0xE)
#define SPI_MODE_GET(_mode_)                    \
        ((_mode_) & SPI_MODE_MASK)

/**
 * @brief SPI Transfer modes (host controller dependent)
 */
#define SPI_TRANSFER_MSB        (0)
#define SPI_TRANSFER_LSB        BIT(4)

/**
 * @brief SPI word size
 */
#define SPI_WORD_SIZE_SHIFT     (5)
#define SPI_WORD_SIZE_MASK      (0x3F << SPI_WORD_SIZE_SHIFT)
#define SPI_WORD_SIZE_GET(_operation_)                                  \
        (((_operation_) & SPI_WORD_SIZE_MASK) >> SPI_WORD_SIZE_SHIFT)

#define SPI_WORD_SET(_word_size_)               \
        ((_word_size_) << SPI_WORD_SIZE_SHIFT)

/**
 * @brief SPI MISO lines
 *
 * Some controllers support dual, quad or octal MISO lines connected to slaves.
 * Default is single, which is the case most of the time.
 */
#define SPI_LINES_SINGLE        (0 << 11)
#define SPI_LINES_DUAL          (1 << 11)
#define SPI_LINES_QUAD          (2 << 11)
#define SPI_LINES_OCTAL         (3 << 11)

#define SPI_LINES_MASK          (0x3 << 11)

/**
 * @brief Specific SPI devices control bits
 */
/* Requests - if possible - to keep CS asserted after the transaction */
#define SPI_HOLD_ON_CS          BIT(13)
/* Keep the device locked after the transaction for the current config.
 * Use this with extreme caution (see spi_release() below) as it will
 * prevent other callers to access the SPI device until spi_release() is
 * properly called.
 */
#define SPI_LOCK_ON             BIT(14)

/* Active high logic on CS - Usually, and by default, CS logic is active
 * low. However, some devices may require the reverse logic: active high.
 * This bit will request the controller to use that logic. Note that not
 * all controllers are able to handle that natively. In this case deferring
 * the CS control to a gpio line through struct spi_cs_control would be
 * the solution.
 */
#define SPI_CS_ACTIVE_HIGH      BIT(15)

/**
 * @brief SPI controller configuration structure
 *
 * @param frequency is the bus frequency in Hertz
 * @param operation is a bit field with the following parts:
 *
 *     operational mode    [ 0 ]       - master or slave.
 *     mode                [ 1 : 3 ]   - Polarity, phase and loop mode.
 *     transfer            [ 4 ]       - LSB or MSB first.
 *     word_size           [ 5 : 10 ]  - Size of a data frame in bits.
 *     lines               [ 11 : 12 ] - MISO lines: Single/Dual/Quad/Octal.
 *     cs_hold             [ 13 ]      - Hold on the CS line if possible.
 *     lock_on             [ 14 ]      - Keep resource locked for the caller.
 *     cs_active_high      [ 15 ]      - Active high CS logic.
 * @param slave is the slave number from 0 to host controller slave limit.
 * @param cs is a valid pointer on a struct spi_cs_control is CS line is
 *    emulated through a gpio line, or NULL otherwise.
 *
 * @note Only cs_hold and lock_on can be changed between consecutive
 * transceive call. Rest of the attributes are not meant to be tweaked.
 *
 * @warning Most drivers use pointer comparison to determine whether a
 * passed configuration is different from one used in a previous
 * transaction.  Changes to fields in the structure may not be
 * detected.
 */
struct spi_config {
	uint32_t operation;
	uint32_t frequency;
	uint16_t slave;
};

struct spi_buf {
        uint8_t *buf;
        size_t len;
};
struct spi_buf_set {
        const struct spi_buf *buffers;
        size_t count;
};

int spi_transceive(const struct device *dev, const struct spi_config *cfg, const struct spi_buf_set *tx, const struct spi_buf_set *rx);

static inline int spi_write(const struct device *dev, const struct spi_config *cfg, const struct spi_buf_set *tx) {
	return spi_transceive(dev, cfg, tx, NULL);
}

static inline int spi_read(const struct device *dev, const struct spi_config *cfg, const struct spi_buf_set *rx) {
	return spi_transceive(dev, cfg, NULL, rx);
}

#endif

