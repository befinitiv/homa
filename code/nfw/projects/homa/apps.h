/*
 * apps.h
 *
 *  Created on: 27.06.2014
 *      Author: renate
 */

#ifndef APPS_H_
#define APPS_H_

struct __attribute__ ((__packed__)) node_status_package {
	uint16_t vdda; //both vdda and temp have 8 fractional bits
	int16_t temp;
	uint8_t node_id;
	uint32_t wakeup_count;
};

void app_uart_gateway(void);
void app_temperature_sensor(void);
void app_wallsocket(void);


#endif /* APPS_H_ */
