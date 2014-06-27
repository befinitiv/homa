#include <libopencm3/stm32/usart.h>


#include "hwdefines.h"
#include "spi.h"
#include "nrf.h"
#include "rtc.h"
#include "pwr.h"
#include "adc.h"
#include "gpio.h"
#include "usart.h"

struct __attribute__ ((__packed__)) app_temperature_sensor_package {
	float vdda;
	float temp;
	uint8_t node_id;
};

void app_uart_gateway(void)
{
	gpio_setup();
	spi_init();
	nrf_init(9);
	nrf_standby_2(1);
	usart_init();

	for(;;) {
		uint8_t reg;
		reg = 0x7e; // clear interrupts
		reg = nrf_write_register(STATUS, &reg, 1);

		//read fifo
		if((reg & 0xe) != 0xe) {
			struct app_temperature_sensor_package pkg;
			nrf_read_rx_payload((uint8_t*)&pkg, sizeof(pkg));
			gpio_toggle(PORT_LED, PIN_LED_G);
			usart_send_blocking(USART1, '-');
		}
	}


}





///* Using API function gpio_toggle(): */
//gpio_toggle(PORT_LED, PIN_LED_G);	/* LED on/off */
//
//
//gpio_set(PORT_LED, PIN_LED_R);
//timer_sleep_us(1000);
//gpio_clear(PORT_LED, PIN_LED_R);
//
//for (i = 0; i < 100000; i++) {	/* Wait a bit. */
//	__asm__("nop");
//}
//
//nrf_test();
//rtc_test();
