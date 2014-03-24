#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/rcc.h>


#include "spi.h"

void spi_hardware_init(void) {
	rcc_periph_clock_enable(RCC_SPI1);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);

	gpio_set_af(GPIOB, 0, GPIO3 | GPIO4 | GPIO5);
	gpio_set_af(GPIOA, 0, GPIO15);

	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3 | GPIO5);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO15);

	spi_reset(SPI1_I2S1_BASE);

	spi_init_master(SPI1_I2S1_BASE, 0, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_1, 0, SPI_CR1_LSBFIRST);


	spi_set_data_size(SPI1_I2S1_BASE, 8);
	spi_enable_ss_output(SPI1_I2S1_BASE);

	SPI_CR2(SPI1_I2S1_BASE) |= SPI_CR2_NSSP;

	spi_enable(SPI1_I2S1_BASE);
}


void spi_init(void) {
	spi_hardware_init();
}


void spi_test(void) {
	spi_send8(SPI1_I2S1_BASE, 0x55);
}
