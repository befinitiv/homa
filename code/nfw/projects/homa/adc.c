/*
 * adc.c
 *
 *  Created on: 07.04.2014
 *      Author: renate
 */


#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

uint8_t channel_array[] = { ADC_CHANNEL1, ADC_CHANNEL1, ADC_CHANNEL_TEMP};

void adc_setup(void)
{
	rcc_periph_clock_enable(RCC_ADC);
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0);
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);

	adc_power_off(ADC1);
	adc_set_clk_source(ADC1, ADC_CLKSOURCE_ADC);
	adc_calibrate_start(ADC1);
	adc_calibrate_wait_finish(ADC1);
	adc_set_operation_mode(ADC1, ADC_MODE_SCAN);
	adc_disable_external_trigger_regular(ADC1);
	adc_set_right_aligned(ADC1);
	adc_enable_temperature_sensor();
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPTIME_071DOT5);
	adc_set_regular_sequence(ADC1, 1, channel_array);
	adc_set_resolution(ADC1, ADC_RESOLUTION_12BIT);
	adc_disable_analog_watchdog(ADC1);
	adc_power_on(ADC1);

	/* Wait for ADC starting up. */
	int i;
	for (i = 0; i < 800000; i++) {    /* Wait a bit. */
		__asm__("nop");
	}

}

void adc_set_channel(uint8_t channel) {
	ADC1_CHSELR = (1 << channel);
}

void adc_init(void) {
	//enable clock to adc
	RCC_APB2ENR |= RCC_APB2ENR_ADCEN;

	//reset ADC
	RCC_APB2RSTR |= RCC_APB2RSTR_ADCRST;
	RCC_APB2RSTR &= ~RCC_APB2RSTR_ADCRST;



	//calibrate adc
	ADC1_CR |= ADC_CR_ADCAL;
	while(ADC1_CR & ADC_CR_ADCAL);

	//enable wait mode
	ADC1_CFGR1 |= ADC_CFGR1_WAIT;

	//239.5 ADC clock cycles conversion time
	ADC1_TR = 7;


	//enable adc
	ADC1_CR |= ADC_CR_ADEN;
	while(!(ADC1_ISR & ADC_ISR_ADRDY));
}

uint16_t adc_read_channel(uint8_t channel) {
	uint16_t retval;



	adc_start_conversion_regular(ADC1);
	while (!(adc_eoc(ADC1)));

	uint16_t temp = adc_read_regular(ADC1);


	uint16_t ts_cal1, ts_cal2;
	ts_cal1 = *((uint16_t*)0x1FFFF7B8);
	ts_cal2 = *((uint16_t*)0x1FFFF7C2);


	float t = (float)(110 - 30) /  (ts_cal2 - ts_cal1) * (temp - ts_cal1) + 30;

//	adc_set_dual_mode(ADC_CR1_DUALMOD_IND);
//	adc_disable_scan_mode(ADC1);
//	adc_set_single_conversion_mode(ADC1);
//	adc_set_sample_time(ADC1, ADC_CHANNEL0, ADC_SMPR1_SMP_1DOT5CYC);
//	adc_set_single_channel(ADC1, ADC_CHANNEL0);
//	adc_enable_trigger(ADC1, ADC_CR2_EXTSEL_SWSTART);
//	adc_power_on(ADC1);
//	adc_reset_calibration(ADC1);
//	adc_calibration(ADC1);
//	adc_start_conversion_regular(ADC1);
//	while (! adc_eoc(ADC1));
//	reg16 = adc_read_regular(ADC1);

	}
