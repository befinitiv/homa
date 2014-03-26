#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/rcc.h>


#include "spi.h"

#define PORT_SPI GPIOB
#define PIN_MOSI GPIO5
#define PIN_MISO GPIO4
#define PIN_SCK GPIO3

#define PORT_SS GPIOA
#define PIN_SS GPIO15


void spi_select(void) {
	gpio_clear(PORT_SS, PIN_SS);
}

void spi_deselect(void) {
	gpio_set(PORT_SS, PIN_SS);
}

void spi_init_hardware(void) {
	rcc_periph_clock_enable(RCC_SPI1);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);

	gpio_set_af(PORT_SPI, 0, PIN_MOSI | PIN_MISO | PIN_SCK);
	//gpio_set_af(PORT_SS, 0, PIN_SS);

	gpio_mode_setup(PORT_SPI, GPIO_MODE_AF, GPIO_PUPD_NONE, PIN_MOSI | PIN_MISO | PIN_SCK);
	//gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO15);
	gpio_mode_setup(PORT_SS, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_SS);

	spi_reset(SPI1_I2S1_BASE);

	spi_init_master(SPI1_I2S1_BASE, 0, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_1, 0, SPI_CR1_LSBFIRST);


	spi_send_msb_first(SPI1_I2S1_BASE);
	spi_set_data_size(SPI1_I2S1_BASE, 8);
	spi_fifo_reception_threshold_8bit(SPI1_I2S1_BASE);



	//spi_enable_ss_output(SPI1_I2S1_BASE);
	//SPI_CR2(SPI1_I2S1_BASE) |= SPI_CR2_NSSP;

	//for some reason the harware SS control does not work as described in the manual.
	//it only outputs pulses if NSSP=1, otherwise it stays constant at 0V
	//RM0091 pp718/914 says that it should go HIGH if SPE=0. This is not the case
	//thus we use GPIO as SS
	spi_enable_software_slave_management(SPI1_I2S1_BASE);
	spi_set_nss_high(SPI1_I2S1_BASE);

	spi_deselect();

	spi_enable(SPI1_I2S1_BASE);
}



void spi_init(void) {
	spi_init_hardware();
}


uint8_t spi_xfer8(uint8_t data)
{
	SPI_DR8(SPI1_I2S1_BASE) = data;

	/* Wait for transfer finished. */
	while (!(SPI_SR(SPI1_I2S1_BASE) & SPI_SR_RXNE));

	/* Read the data from DR. */
	return SPI_DR8(SPI1_I2S1_BASE);
}


void spi_xfer8n(uint8_t len, const uint8_t *tx, uint8_t *rx) {
	if(rx && tx) {
		while(len--) {
			*rx++ = spi_xfer8(*tx++);
		}
	}
	else if(tx)
	{
		while(len--) {
			spi_xfer8(*tx++);
		}
	}
	else
	{
		while(len--) {
			*rx++ = spi_xfer8(0);
		}
	}
}

void spi_transfer(uint8_t len, const uint8_t *tx, uint8_t *rx) {
	spi_select();
	spi_xfer8n(len, tx, rx);
	spi_deselect();
}

int spi_test(void) {
	uint8_t c = 0xff;

	spi_transfer(1, &c, &c);

	return c;
}
