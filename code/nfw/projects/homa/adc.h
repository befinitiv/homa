/*
 * adc.h
 *
 *  Created on: 23.06.2014
 *      Author: renate
 */

#ifndef ADC_H_
#define ADC_H_

#include <libopencm3/stm32/adc.h>


void adc_init(uint8_t channel_count, uint8_t *channels);
void adc_get_temp(float *vdda, float *temp);


#endif /* ADC_H_ */
