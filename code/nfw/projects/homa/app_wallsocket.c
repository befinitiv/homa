/*
 * app_wallsocket.c
 *
 *  Created on: 27.06.2014
 *      Author: renate
 */

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "hwdefines.h"
#include "nrf24l01.h"
#include "nrf.h"
#include "timer.h"
#include "spi.h"
#include "gpio.h"

#include "apps.h"

void app_wallsocket(void) {
	gpio_setup();
	spi_init();
	nrf_init(2);
	nrf_standby_2(1);

	for(;;) {
		uint8_t reg;
		reg = 0x7e; // clear interrupts
		reg = nrf_write_register(STATUS, &reg, 1);

		//read fifo
		if((reg & 0xe) != 0xe) {
			uint8_t p[2];
			volatile uint16_t key;
			nrf_read_rx_payload(p, sizeof(p));
			key = (*(uint16_t*)p) & 0xff;

			//bottom
			if(key == 0x1e) {
				gpio_clear(PORT_LED, PIN_LED_R);
				gpio_clear(PORT_IO_0, PIN_IO_0);
			} else if(key == 0xf) { //middle
				gpio_set(PORT_LED, PIN_LED_R);
				gpio_set(PORT_IO_0, PIN_IO_0);
			}
		}

		gpio_toggle(PORT_LED, PIN_LED_G);
		timer_sleep_ms(50);
	}
}
