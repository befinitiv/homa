/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Copyright (C) 2012 Karl Palsson <karlp@tweak.net.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define PORT_LED GPIOA
#define PIN_LED GPIO6
#define PIN_LED_R GPIO7

#include "spi.h"
#include "nrf.h"


static void gpio_setup(void)
{
	/* Enable GPIOB clock. */
	/* Manually: */
	//RCC_AHBENR |= RCC_AHBENR_GPIOCEN;
	/* Using API functions: */
	rcc_periph_clock_enable(RCC_GPIOA);


	/* Set GPIO6 (in GPIO port B) to 'output push-pull'. */
	/* Using API functions: */
	gpio_mode_setup(PORT_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED | PIN_LED_R);
}

int main(void)
{
	int i;

	spi_init();
	nrf_init();
	nrf_standby_2(1);

	gpio_setup();


	/* Blink the LED (PC8) on the board. */
	while (1) {
		/* Manually: */
		// GPIOC_BSRR = PIN_LED;		/* LED off */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");
		// GPIOC_BRR = PIN_LED;		/* LED on */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");

		/* Using API functions gpio_set()/gpio_clear(): */
		// gpio_set(PORT_LED, PIN_LED);	/* LED off */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");
		// gpio_clear(PORT_LED, PIN_LED);	/* LED on */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");

		/* Using API function gpio_toggle(): */
		gpio_toggle(PORT_LED, PIN_LED);	/* LED on/off */
		for (i = 0; i < 100000; i++) {	/* Wait a bit. */
			__asm__("nop");
		}

		if(nrf_test()) {
			gpio_toggle(PORT_LED, PIN_LED_R);
		}
	}

	return 0;
}
