#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>



#include "nrf24l01.h"
#include "nrf.h"
#include "spi.h"


#define PORT_CE GPIOA
#define PIN_CE GPIO12
#define PORT_IRQ GPIOA
#define PIN_IRQ GPIO11



uint8_t reg_config = 0;



void nrf_ce(void) {
	gpio_set(PORT_CE, PIN_CE);
}

void nrf_nce(void) {
	gpio_clear(PORT_CE, PIN_CE);
}

void nrf_init_hardware(void) {
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_mode_setup(PORT_CE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_CE);

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



void nrf_init(void) {
	nrf_init_hardware();

	reg_config |= (1 << EN_CRC);
	nrf_pwr_down();

	uint8_t reg;

	reg = 3; // 3 byte addresses
	nrf_write_register(SETUP_AW, &reg, 1);

	reg = 0; //disable automatic retransmit
	nrf_write_register(SETUP_RETR, &reg, 1);

	reg = 0; //disable auto retransmit
	nrf_write_register(EN_AA, &reg, 1);

	reg = (3 << RF_PWR); //0dBm power, 1mbps
	nrf_write_register(RF_SETUP, &reg, 1);

	reg = 4;
	nrf_write_register(RX_PW_P0, &reg, 1);

}


uint8_t nrf_test(void) {
	uint8_t reg;

	reg = 0x7e; // clear interrupts
	reg = nrf_write_register(STATUS, &reg, 1);

	//read fifo
	if((reg & 0xe) != 0xe) {
		uint8_t p[4];
		nrf_read_rx_payload(p, sizeof(p));
	}

	return reg & 64;
}
