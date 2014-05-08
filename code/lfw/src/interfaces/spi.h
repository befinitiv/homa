#ifndef SPI_H
#define SPI_H

/*
 * parameter:
 * 1: enable = 1; disable = 0
 */
void spi_slaveSelect(int enable);

/*
 * parameters:
 * 1: length
 * 2: tx-buffer (null for rx only)
 * 3: rx-buffer (null for tx only)
 * return: amount of data read
 */
uint8_t spi_xfer(uint8_t len, const uint8_t *tx, uint8_t *rx);

/*
 * flush the spi interface (in case some transactions are ongoing)
 */
void spi_flush();


#endif
