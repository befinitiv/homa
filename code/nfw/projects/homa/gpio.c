#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/pwr.h>

#include "hwdefines.h"
#include "gpio.h"

void gpio_setup(void)
{
	/* Enable GPIOB clock. */
	/* Manually: */
	//RCC_AHBENR |= RCC_AHBENR_GPIOCEN;
	/* Using API functions: */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);

	//enable pulldown to save power (this disables the debugger!)
#ifndef TESTING
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_ALL);
	gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_ALL);
#endif

	gpio_mode_setup(PORT_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED_G | PIN_LED_R);
	gpio_mode_setup(PORT_IO_0, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_IO_0);

	gpio_clear(PORT_IO_0, PIN_IO_0);
	gpio_clear(PORT_LED, PIN_LED_G | PIN_LED_R);
}
