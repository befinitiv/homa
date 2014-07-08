/*
 * protocol.h
 *
 *  Created on: 28.06.2014
 *      Author: renate
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

struct __attribute__ ((__packed__)) node_status_package {
	uint8_t node_id;
	uint32_t wakeup_count;
	uint16_t vdda; //both vdda and temp have 8 fractional bits
	int16_t temp;
};


struct __attribute__ ((__packed__)) advertise_package {
	uint32_t unique_id[3];
	uint8_t app_id;
};


void protocol_send_status_package(uint8_t node_id, uint32_t wakeup_count);
void protocol_send_advertise_package(uint8_t app_id);

#endif /* PROTOCOL_H_ */
