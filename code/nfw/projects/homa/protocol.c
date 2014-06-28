/*
 * protocol.c
 *
 *  Created on: 28.06.2014
 *      Author: renate
 */

#include <stdint.h>

#include <libopencm3/stm32/memorymap.h>

#include "hwdefines.h"
#include "spi.h"
#include "nrf.h"
#include "rtc.h"
#include "pwr.h"
#include "adc.h"
#include "gpio.h"

#include "protocol.h"

uint32_t *unique_id_96bit = (uint32_t*)DESIG_UNIQUE_ID_BASE;


void protocol_send_status_package(uint8_t node_id, uint32_t wakeup_count) {
	uint8_t channel_conf[] = { ADC_CHANNEL_TEMP, ADC_CHANNEL_VREF};
	adc_init(2, channel_conf);


	struct node_status_package pkg;

	float vdda, temp;
	adc_get_temp(&vdda, &temp);

	pkg.node_id = node_id;
	pkg.wakeup_count = wakeup_count;
	pkg.temp = temp * 256;
	pkg.vdda = vdda * 256;

	nrf_write_tx_payload((uint8_t*)&pkg, sizeof(pkg));
}


void protocol_send_advertise_package(uint8_t app_id) {
	struct advertise_package pkg;

	pkg.unique_id[0] = unique_id_96bit[0];
	pkg.unique_id[1] = unique_id_96bit[1];
	pkg.unique_id[2] = unique_id_96bit[2];

	pkg.app_id = app_id;

	nrf_write_tx_payload((uint8_t*)&pkg, sizeof(pkg));
}
