#include "nrf24l01.h"


#define NRF_RX_FIFO_EMPTY (0x7)
#define NRF_RX_P_NO(status) ((status & 0xe) >> 1)

#define NRF_CLEAR_ALL_INTERRUPTS ((uint8_t)((1 << RX_DR) | (1 << TX_DS) << (1 << MAX_RT)))

void nrf_init(uint8_t payload_size);
void nrf_standby_2(uint8_t rx);
void nrf_standby_1(void);
void nrf_pwr_down(void);

uint8_t nrf_write_tx_payload(const uint8_t *data, uint8_t len);
uint8_t nrf_write_register(uint8_t addr, const uint8_t *data, uint8_t len);
uint8_t nrf_read_rx_payload(uint8_t *data, uint8_t len);


