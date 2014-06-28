#include <libopencm3/stm32/usart.h>


#include "hwdefines.h"
#include "spi.h"
#include "nrf.h"
#include "rtc.h"
#include "pwr.h"
#include "adc.h"
#include "gpio.h"
#include "usart.h"

#include "protocol.h"


void uart_send_pkg(uint8_t *data, uint8_t len) {
	uint8_t chk = len;
	usart_send_blocking(USART1, 0xff);
	usart_send_blocking(USART1, 0xff);
	usart_send_blocking(USART1, len);
	while(len-- > 0) {
		usart_send_blocking(USART1, *data);
		chk += *data++;
	}
	usart_send_blocking(USART1, chk);

}


void app_uart_gateway(void)
{
	gpio_setup();
	spi_init();
	nrf_init(sizeof(struct node_status_package));
	nrf_standby_2(1);
	usart_init();

	for(;;) {
		uint8_t reg;
		reg = NRF_CLEAR_ALL_INTERRUPTS;
		reg = nrf_write_register(STATUS, &reg, 1);

		//check which rx pipe has data
		if(NRF_RX_P_NO(reg) != NRF_RX_FIFO_EMPTY) {
			struct node_status_package pkg;

			nrf_read_rx_payload((uint8_t*)&pkg, sizeof(pkg));
//			float vdda, temp;
//			temp = 1.0 * pkg.temp / 256;
//			vdda = 1.0 * pkg.vdda / 256;

			gpio_toggle(PORT_LED, PIN_LED_G);
			uart_send_pkg((uint8_t*)&pkg, sizeof(pkg));
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
