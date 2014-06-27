#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "hwdefines.h"
#include "nrf24l01.h"
#include "nrf.h"
#include "spi.h"
#include "pwr.h"




uint8_t reg_config = 0;



void nrf_ce(void) {
	gpio_set(PORT_RF_CE, PIN_RF_CE);
}

void nrf_nce(void) {
	gpio_clear(PORT_RF_CE, PIN_RF_CE);
}

void nrf_init_hardware(void) {
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_mode_setup(PORT_RF_CE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_RF_CE);



	nrf_nce();
}


uint8_t nrf_write_data(uint8_t command, const uint8_t *data, uint8_t len) {
	uint8_t status;

	spi_select();
	status = spi_xfer8(command);
	spi_xfer8n(len, data, (void*)0);
	spi_deselect();

	return status;
}

uint8_t nrf_read_data(uint8_t command, uint8_t *data, uint8_t len) {
	uint8_t status;

	spi_select();
	status = spi_xfer8(command);
	spi_xfer8n(len, (void*)0, data);
	spi_deselect();

	return status;
}


uint8_t nrf_read_register(uint8_t addr, uint8_t *data, uint8_t len) {
	return nrf_read_data(R_REGISTER + addr, data, len);
}

uint8_t nrf_write_register(uint8_t addr, const uint8_t *data, uint8_t len) {
	return nrf_write_data(W_REGISTER + addr, data, len);
}

uint8_t nrf_write_tx_payload(const uint8_t *data, uint8_t len) {
	return nrf_write_data(W_TX_PAYLOAD, data, len);
}

uint8_t nrf_read_rx_payload(uint8_t *data, uint8_t len) {
	return nrf_read_data(R_RX_PAYLOAD, data, len);
}

void nrf_pwr_down(void) {
	nrf_nce();
	reg_config &= ~(1 << PWR_UP);
	nrf_write_register(CONFIG, &reg_config, 1);
}

void nrf_standby_1(void) {
	nrf_nce();
	reg_config |= (1 << PWR_UP);
	nrf_write_register(CONFIG, &reg_config, 1);
}

void nrf_standby_2(uint8_t rx) {
	nrf_ce();
	reg_config |= (1 << PWR_UP) + rx;
	nrf_write_register(CONFIG, &reg_config, 1);
}


void nrf_init_chip_settings(uint8_t payload_size) {
	uint8_t reg;

	nrf_write_register(CONFIG, &reg_config, 1);

	reg = 1; // 3 byte addresses
	nrf_write_register(SETUP_AW, &reg, 1);

	reg = 0; //disable automatic retransmit
	nrf_write_register(SETUP_RETR, &reg, 1);

	reg = 0; //disable auto retransmit
	nrf_write_register(EN_AA, &reg, 1);

	reg = (3 << RF_PWR) | (1 << RF_DR_LOW); //0dBm power, 250kbps
	nrf_write_register(RF_SETUP, &reg, 1);

	reg = payload_size;
	nrf_write_register(RX_PW_P0, &reg, 1);
}

void nrf_init(uint8_t payload_size) {
	nrf_init_hardware();

	reg_config = (1 << EN_CRC);

	if(!PWR_WAS_I_IN_STANDBY) {
		nrf_init_chip_settings(payload_size);
	}

}
