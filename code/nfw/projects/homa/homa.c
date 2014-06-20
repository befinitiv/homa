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
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencmsis/core_cm3.h>
#include <libopencm3/stm32/flash.h>


#include "hwdefines.h"
#include "spi.h"
#include "nrf.h"
#include "rtc.h"
#include "pwr.h"

#define TESTING

static void gpio_setup(void)
{
	/* Enable GPIOB clock. */
	/* Manually: */
	//RCC_AHBENR |= RCC_AHBENR_GPIOCEN;
	/* Using API functions: */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);


	/* Set GPIO6 (in GPIO port B) to 'output push-pull'. */
	/* Using API functions: */

	//enable pulldown to save power
#ifndef TESTING
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_ALL);
	gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_ALL);
#endif

	gpio_mode_setup(PORT_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED_G | PIN_LED_R);
	gpio_mode_setup(PORT_IO_0, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_IO_0);

	gpio_clear(PORT_IO_0, PIN_IO_0);
	gpio_set(PORT_LED, PIN_LED_G | PIN_LED_R);
}



int main(void)
{
	int i, t;


	if(!PWR_WAS_I_IN_STANDBY) {
		//sleep a bit. this helps the voltage of the radio to settle. also you are not yet locked out of JTAG (which could happen if the chip goes immediately into standby mode)
		for (i = 0; i < 1000000; i++) {	/* Wait a bit. */
			__asm__("nop");
		}
	}

	rtc_init();
	gpio_setup();
	spi_init();
	nrf_init();
	nrf_standby_2(1);


	/* Blink the LED (PC8) on the board. */
	for(;;){ //for(t=0; t<20; ++t) {

		/* Using API function gpio_toggle(): */
		gpio_toggle(PORT_LED, PIN_LED_G);	/* LED on/off */


		gpio_set(PORT_LED, PIN_LED_R);
		timer_sleep_us(50000);
		gpio_clear(PORT_LED, PIN_LED_R);

		for (i = 0; i < 100000; i++) {	/* Wait a bit. */
			__asm__("nop");
		}

		nrf_test();
		rtc_test();
		//adc_read_channel(16);
	}


	//gpio_clear(PORT_LED, PIN_LED_G | PIN_LED_R);
#ifndef TESTING
	pwr_enter_standby();
#endif


	return 0;
}
