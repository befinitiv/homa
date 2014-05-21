#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/rcc.h>

#include <flawless/init/systemInitializer.h>
#include <interfaces/spi.h>

#define PORT_SPI GPIOB
#define PIN_MOSI GPIO5
#define PIN_MISO GPIO4
#define PIN_SCK GPIO3

#define PORT_SS GPIOA
#define PIN_SS GPIO15

#define SPI_INTERFACE SPI1


void spi_slaveSelect(int enable) {
	if (enable) {
		gpio_clear(PORT_SS, PIN_SS);
	} else {
		gpio_set(PORT_SS, PIN_SS);
	}
}


static uint8_t spi_xfer8(uint8_t data)
{
	SPI_DR8(SPI1_I2S1_BASE) = data;

	/* Wait for transfer finished. */
	while (!(SPI_SR(SPI1_I2S1_BASE) & SPI_SR_RXNE));

	/* Read the data from DR. */
	return SPI_DR8(SPI1_I2S1_BASE);
}


uint8_t spi_xfer(uint8_t len, const uint8_t *tx, uint8_t *rx) {
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

	return len;
}

void spi_flush()
{
	while (0 == (SPI_SR(SPI_INTERFACE) & (SPI_SR_TXE))); // wait for txe finish
	while (0 != (SPI_SR(SPI_INTERFACE) & SPI_SR_RXNE))
	{
		(void) SPI_DR(SPI_INTERFACE); // read stuff
	}
}


static void spi_init_hardware(void);
MODULE_INIT_FUNCTION(spiDriver, 3, spi_init_hardware)
static void spi_init_hardware(void) {
	RCC_APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC_AHBENR  |= RCC_AHBENR_GPIOAEN;
	RCC_AHBENR  |= RCC_AHBENR_GPIOBEN;

	gpio_set_af(PORT_SPI, GPIO_AF0, PIN_MOSI | PIN_MISO | PIN_SCK);

	gpio_mode_setup(PORT_SPI, GPIO_MODE_AF, GPIO_PUPD_NONE, PIN_MOSI | PIN_MISO | PIN_SCK);
	gpio_mode_setup(PORT_SS, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_SS);

//	spi_reset(SPI1_I2S1_BASE);
	SPI_CR1(SPI_INTERFACE) = 0;
	SPI_CR2(SPI_INTERFACE) = 0;

//	spi_init_master(SPI1_I2S1_BASE, 0, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_1, 0, SPI_CR1_LSBFIRST);
	SPI_CR1(SPI_INTERFACE) = SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE | SPI_CR1_CPHA_CLK_TRANSITION_1 | SPI_CR1_MSBFIRST | SPI_CR1_MSTR;
	SPI_CR2(SPI_INTERFACE) = SPI_CR2_DS_8BIT | SPI_CR2_FRXTH;


	SPI_CR1(SPI_INTERFACE) |= SPI_CR1_SSM;
	SPI_CR1(SPI_INTERFACE) |= SPI_CR1_SSI;

	spi_slaveSelect(0);

	SPI_CR1(SPI_INTERFACE) |= SPI_CR1_SPE;
}

