#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/rcc.h>


#include "spi.h"

void spi_hardware_init(void) {
	rcc_periph_clock_enable(RCC_SPI1);

	gpio_set_af(GPIOB, 0, GPIO3 | GPIO4 | GPIO5);
	gpio_set_af(GPIOA, 0, GPIO15);

	spi_reset(SPI1_I2S1_BASE);

}


void spi_init(void) {
	spi_hardware_init();
}
