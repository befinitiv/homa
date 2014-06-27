/*
 * adc.c
 *
 *  Created on: 07.04.2014
 *      Author: renate
 */


#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define TS_CAL1     *((uint16_t*) 0x1FFFF7B8)
#define TS_CAL2     *((uint16_t*) 0x1FFFF7C2)
#define AVG_SLOPE   (float)((TS_CAL1 - TS_CAL2)/80.0)
#define VREFINT_CAL    *((uint16_t*) 0x1FFFF7BA)


void adc_init(uint8_t channel_count, const uint8_t *channels)
{
	int i;

	rcc_periph_clock_enable(RCC_ADC);
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0);
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);

	adc_power_off(ADC1);
	adc_set_clk_source(ADC1, ADC_CLKSOURCE_ADC);
	adc_set_operation_mode(ADC1, ADC_MODE_SEQUENTIAL);
	adc_disable_external_trigger_regular(ADC1);
	adc_set_right_aligned(ADC1);
	adc_enable_temperature_sensor();
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPTIME_071DOT5);
	adc_set_regular_sequence(ADC1, channel_count, channels);
	adc_set_resolution(ADC1, ADC_RESOLUTION_12BIT);
	adc_disable_analog_watchdog(ADC1);

	for(i=0; i<channel_count; ++i) {
		switch(channels[i]) {
		case ADC_CHANNEL_TEMP:
			adc_enable_temperature_sensor();
			break;
		case ADC_CHANNEL_VBAT:
			adc_enable_vbat_sensor();
			break;
		case ADC_CHANNEL_VREF:
			adc_enable_vref_sensor();
		default:
			break;
		}
	}


	adc_calibrate_start(ADC1);
	adc_calibrate_wait_finish(ADC1);

	adc_power_on(ADC1);

	/* Wait for ADC starting up. */
	while(!(ADC1_ISR & ADC_ISR_ADRDY));

}




uint16_t adc_read_channels(uint8_t channel_count, uint16_t *channel_data) {




	while(channel_count--) {
		adc_start_conversion_regular(ADC1);
		while (!(adc_eoc(ADC1)));

		*channel_data++ = adc_read_regular(ADC1);
	}


}

float adc_get_vdda(uint16_t vref) {
	return 3.3 * VREFINT_CAL / vref;
}

void adc_test() {
	uint16_t channels[2];

	adc_read_channels(2, channels);

	float vdda = adc_get_vdda(channels[1]);
	float temp = (((TS_CAL1 - vdda / 3.3 * channels[0])/AVG_SLOPE)+25);
}
