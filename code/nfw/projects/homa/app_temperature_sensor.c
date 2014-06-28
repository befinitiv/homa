#include <libopencm3/stm32/rtc.h>


#include "hwdefines.h"
#include "spi.h"
#include "nrf.h"
#include "rtc.h"
#include "pwr.h"
#include "adc.h"
#include "gpio.h"

#include "apps.h"


 struct __attribute__ ((__packed__)) nv_data_type {
	uint8_t node_id;
	uint32_t wakeup_count;
};


 static volatile struct nv_data_type *nv_data = (struct nv_data_type*)RTC_BKP_BASE;


void app_temperature_sensor(void)
{
	rtc_init();
	rtc_enable_periodic_alarm(0, 5);
	gpio_setup();
	spi_init();
	nrf_init(sizeof(struct node_status_package));
	nrf_standby_2(0);


	nv_data->wakeup_count++;

	uint8_t channel_conf[] = { ADC_CHANNEL_TEMP, ADC_CHANNEL_VREF};
	adc_init(2, channel_conf);


	struct node_status_package pkg;

	float vdda, temp;
	adc_get_temp(&vdda, &temp);

	pkg.node_id = 123;
	pkg.wakeup_count = nv_data->wakeup_count;
	pkg.temp = temp * 256;
	pkg.vdda = vdda * 256;

	nrf_write_tx_payload((uint8_t*)&pkg, sizeof(pkg));

	pwr_enter_standby();


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
