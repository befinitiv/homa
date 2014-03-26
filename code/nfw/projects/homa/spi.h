#ifndef SPI_H
#define SPI_H



void spi_init(void);
void spi_select(void);
void spi_deselect(void);
uint8_t spi_xfer8(uint8_t data);
void spi_xfer8n(uint8_t len, const uint8_t *tx, uint8_t *rx);
void spi_transfer(uint8_t len, const uint8_t *tx, uint8_t *rx);

int spi_test(void);

#endif
