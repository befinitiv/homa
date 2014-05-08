/*
 * nrf.h
 *
 *  Created on: 06.05.2014
 *      Author: lutz
 */

#ifndef NRF_H_
#define NRF_H_

#include <flawless/stdtypes.h>

typedef uint8_t nrfPipeHandle_t;


/*
 * callback for incoming packets
 */
typedef void (*nrfReceiveCallback_t)(nrfPipeHandle_t, const void*, uint8_t);

nrfPipeHandle_t nrf_setupNewRxDataPipe(uint8_t pipeAddr, uint8_t packetLen, nrfReceiveCallback_t receiveCallback, bool autoAck);

void nrf_setRxBaseAddress(void *baseAddress, int len);

void nrf_sendPacket(const void *targetAddr, int targetAddrLen, const void *packetBuf, int packetBufLen, bool autoAck, uint8_t retransmitts);

/************************ Power related stuff *********************/
void nrf_standby_2(uint8_t rx);
void nrf_standby_1(void);
void nrf_pwr_down(void);
void nrf_pwr_up(void);


#endif /* NRF_H_ */
