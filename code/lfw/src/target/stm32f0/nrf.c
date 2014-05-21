#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/f0/nvic.h>

#include "hwdefines.h"
#include "gpio_interrupts.h"

#include <interfaces/nrf.h>
#include <interfaces/nrf24l01.h>
#include <interfaces/spi.h>
#include "pwr.h"

#include <string.h>

#include <flawless/init/systemInitializer.h>




#define NRF_MAX_ADDR_LEN 5
#define NRF_MIN_ADDR_LEN 3
#define NRF_MAX_RX_PIPES 5
#define NRF_MAX_PACKET_LEN 32

static uint8_t g_baseAddress[NRF_MAX_ADDR_LEN] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
static uint8_t g_baseAddressLen = 3;
static uint8_t g_enabledDataPipesMask = 1;
static uint8_t g_enabledAutoAckMask = 0;

typedef struct tag_nrfPipeDescription {
	nrfReceiveCallback_t callback;
	uint8_t packetLen;
} nrfPipeDescription_t;

static nrfPipeDescription_t g_PipeDescriptors[NRF_MAX_RX_PIPES] = { {NULL, 0U} };

static uint8_t g_reg_config = 0;

static void nrf_ce(void) {
	gpio_set(PORT_RF_CE, PIN_RF_CE);
}

static void nrf_nce(void) {
	gpio_clear(PORT_RF_CE, PIN_RF_CE);
}

static uint8_t nrf_write_data(uint8_t command, const uint8_t *data, uint8_t len) {
	uint8_t status;

	spi_slaveSelect(1);
	spi_xfer(sizeof(command), &command, &status);
	spi_xfer(len, data, (void*)0);
	spi_slaveSelect(0);

	return status;
}

static uint8_t nrf_read_data(uint8_t command, uint8_t *data, uint8_t len) {
	uint8_t status;

	spi_slaveSelect(1);
	spi_xfer(sizeof(command), &command, &status);
	spi_xfer(len, (void*)0, data);
	spi_slaveSelect(0);

	return status;
}


static uint8_t nrf_read_register(uint8_t addr, void *data, uint8_t len) {
	return nrf_read_data(NRF_R_REGISTER + addr, data, len);
}

static uint8_t nrf_write_register(uint8_t addr, const void *data, uint8_t len) {
	return nrf_write_data(NRF_W_REGISTER + addr, data, len);
}

static uint8_t nrf_read_rx_payload(void *data, uint8_t len) {
	return nrf_read_data(NRF_R_RX_PAYLOAD, data, len);
}

void nrf_pwr_down(void) {
	nrf_nce();
	g_reg_config &= ~(1 << NRF_PWR_UP);
	nrf_write_register(NRF_CONFIG, &g_reg_config, 1);
}

void nrf_standby_1(void) {
	nrf_nce();
	g_reg_config |= (1 << NRF_PWR_UP);
	nrf_write_register(NRF_CONFIG, &g_reg_config, 1);
}

void nrf_standby_2(uint8_t rx) {
	nrf_ce();
	g_reg_config |= (1 << NRF_PWR_UP) + rx;
	nrf_write_register(NRF_CONFIG, &g_reg_config, 1);
}

void nrf_flushRx()
{
	nrf_read_data(NRF_FLUSH_RX, NULL, 0);
}

void nrf_flushTx()
{
	nrf_read_data(NRF_FLUSH_TX, NULL, 0);
}

static void on_irqLine(void * unused)
{
	uint8_t status = 0;
	nrf_read_register(NRF_STATUS, &status, sizeof(status));

	if (0 != (status & (1 << NRF_RX_DR)))
	{
		uint8_t payload[NRF_MAX_PACKET_LEN];
		uint8_t rxPipe = (status >> 1) & 0x7;
		if (rxPipe < NRF_MAX_RX_PIPES && rxPipe > 0)
		{
			rxPipe -= 1;
			if (NULL != g_PipeDescriptors[rxPipe].callback)
			{
				nrf_read_rx_payload(&payload, g_PipeDescriptors[rxPipe].packetLen);
				(void)(g_PipeDescriptors[rxPipe].callback)(rxPipe,
						payload,
						g_PipeDescriptors[rxPipe].packetLen);
			}
		} else
		{
			nrf_flushRx();
		}
	}

	/* the following is for debugging purposes only (flush all incomming stuff in case there was a breakpoint in a callback function) */
	nrf_read_register(NRF_STATUS, &status, sizeof(status));
	if (0 != (status & (1 << NRF_RX_DR)))
	{
		nrf_flushRx();
	}
	status &= 0x70;
	nrf_write_register(NRF_STATUS, &status, sizeof(status));
	UNUSED(unused);
}

static void nrf_init_chip_settings() {
	uint8_t reg;

	nrf_write_register(NRF_CONFIG, &g_reg_config, 1);

	reg = 1; // 3 byte addresses
	nrf_write_register(NRF_SETUP_AW, &reg, 1);

	reg = 0; //disable automatic retransmit
	nrf_write_register(NRF_SETUP_RETR, &reg, 1);

	reg = 0; //disable auto retransmit
	nrf_write_register(NRF_EN_AA, &reg, 1);

	reg = (3 << NRF_RF_PWR) | (1 << NRF_RF_DR_LOW); //0dBm power, 250kbps
	nrf_write_register(NRF_RF_SETUP, &reg, 1);

	uint8_t p0DataLen = 0;
	nrf_write_register(NRF_RX_PW_P0, &p0DataLen, sizeof(p0DataLen));

	nrf_flushRx();
	nrf_flushTx();
	uint8_t status = 0;
	nrf_read_register(NRF_STATUS, &status, sizeof(status));
	status &= 0x70;
	nrf_write_register(NRF_STATUS, &status, sizeof(status));
}

static void nrf_init_hardware(void)
{
	RCC_AHBENR |= RCC_AHBENR_GPIOAEN;
	gpio_mode_setup(PORT_RF_CE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_RF_CE);
	nrf_nce();
}

void nrf_pwr_up(void)
{
	g_reg_config = (1 << NRF_EN_CRC) | (1 << NRF_PWR_UP) | (1 << NRF_MASK_MAX_RT) | (1 << NRF_MASK_TX_DS);
	nrf_init_hardware();

	nrf_standby_2(1);
}

static void nrf_init(void);
MODULE_INIT_FUNCTION(nrf, 4, nrf_init)
static void nrf_init(void) {
	nrf_init_hardware();
	nrf_pwr_up();

	nrf_init_chip_settings();
	if(!PWR_WAS_I_IN_STANDBY) {
		nrf_init_chip_settings();
	}
	gpio_registerFor_interrupt(&on_irqLine, PORT_RF_IRQ, PIN_RF_IRQ, GPIO_TRIGGER_LEVEL_FALLING, NULL);
	gpio_enable_interrupt(PORT_RF_IRQ, PIN_RF_IRQ);
}

nrfPipeHandle_t nrf_setupNewRxDataPipe(uint8_t pipeAddr, uint8_t packetLen, nrfReceiveCallback_t receiveCallback, bool autoAck)
{
	int i = 0;
	nrfPipeHandle_t retHandle = -1;
	if ((packetLen > 0 && packetLen <= NRF_MAX_PACKET_LEN) &&
		(NULL != receiveCallback))
	{
		for (i = 0; i < NRF_MAX_RX_PIPES; ++i)
		{
			if (NULL == g_PipeDescriptors[i].callback)
			{
				/* pick this pipe */
				g_PipeDescriptors[i].callback = receiveCallback;
				g_PipeDescriptors[i].packetLen = packetLen;

				/* setup the address for this pipe */
				const uint8_t addrRegisterBaseAddr = NRF_RX_ADDR_P1 + i;
				const uint8_t payloadLenBaseAddr = NRF_RX_PW_P1 + i;

				if (0 == i)
				{
					/* this register contains 5 bits (or less... depending on global configuration) */
					uint8_t addr[NRF_MAX_ADDR_LEN];

					ASSERT(g_baseAddressLen >= NRF_MIN_ADDR_LEN);
					ASSERT(g_baseAddressLen <  NRF_MAX_ADDR_LEN);
					memcpy(addr, g_baseAddress, g_baseAddressLen);
					addr[g_baseAddressLen] = pipeAddr;

					nrf_write_register(addrRegisterBaseAddr, addr, g_baseAddressLen + 1);
				} else {
					nrf_write_register(addrRegisterBaseAddr, &pipeAddr, sizeof(pipeAddr));
				}

				nrf_write_register(payloadLenBaseAddr, &packetLen, sizeof(packetLen));

				g_enabledDataPipesMask |= (1 << (i + 1));
				nrf_write_register(NRF_EN_RXADDR, &g_enabledDataPipesMask, sizeof(g_enabledDataPipesMask));

				if (false != autoAck)
				{
					g_enabledAutoAckMask |= (1 << (i + 1));
					nrf_write_register(NRF_EN_AA, &g_enabledAutoAckMask, sizeof(g_enabledAutoAckMask));
				}

				retHandle = i;
				break;
			}
		}
	}
	return retHandle;
}

void nrf_setRxBaseAddress(void *baseAddress, int len)
{
	if (len >= NRF_MIN_ADDR_LEN - 1 && len < NRF_MAX_ADDR_LEN)
	{
		g_baseAddressLen = len;
		memcpy(g_baseAddress, baseAddress, len);

		uint8_t actualLen = g_baseAddressLen - 1;
		nrf_write_register(NRF_SETUP_AW, &actualLen, sizeof(actualLen));
	}
}

void nrf_sendPacket(const void *targetAddr, int targetAddrLen, const void *packetBuf, int packetBufLen, bool autoAck, uint8_t retransmitts)
{
	if ((NULL != targetAddr) &&
		(targetAddrLen == g_baseAddressLen) &&
		(NULL != packetBuf) &&
		(packetBufLen > 0) &&
		(packetBufLen < 33))
	{
		nrf_read_register(NRF_CONFIG, &g_reg_config, sizeof(g_reg_config));
		if (0 != (g_reg_config & (1 << NRF_PRIM_RX))) /* if we are in receive mode */
		{
			g_reg_config &= ~(1 << NRF_PRIM_RX); /* go into transmit mode */
			nrf_write_register(NRF_CONFIG, &g_reg_config, sizeof(g_reg_config));

			if (autoAck) {
				g_enabledAutoAckMask |= (1 << 0);
				nrf_write_register(NRF_EN_AA, &g_enabledAutoAckMask, sizeof(g_enabledAutoAckMask));
				nrf_write_register(NRF_RX_ADDR_P0, targetAddr, targetAddrLen);
			} else {
				g_enabledAutoAckMask &= ~(1 << 0);
				nrf_write_register(NRF_EN_AA, &g_enabledAutoAckMask, sizeof(g_enabledAutoAckMask));
			}

			retransmitts = retransmitts & 0x0f;
			nrf_write_register(NRF_SETUP_RETR, &retransmitts, sizeof(retransmitts));


			nrf_write_register(NRF_TX_ADDR, targetAddr, targetAddrLen);
			nrf_write_data(NRF_W_TX_PAYLOAD, packetBuf, packetBufLen);

			/* trigger sending of packet by pulsing the cs pin */
			gpio_set(PORT_RF_CE, PIN_RF_CE);
			for (unsigned int i = 0; i < (1U << 10); ++i) {
				__asm__ volatile ("nop");
			}
			gpio_clear(PORT_RF_CE, PIN_RF_CE);

			int pinstatus = gpio_get(PORT_RF_IRQ, PIN_RF_IRQ);
			uint8_t statusReg;
			nrf_read_register(NRF_STATUS, &statusReg, sizeof(statusReg));
			__asm__ volatile ("nop");
			UNUSED(pinstatus);
			UNUSED(statusReg);
		} else {
			/* this function was called when we have been in transmission mode! this is most likely to be an error */
		}
	}
}

uint8_t nrf_test(void) {
	uint8_t reg;

	reg = 0x7e; // clear interrupts
	reg = nrf_write_register(NRF_STATUS, &reg, 1);
	nrf_read_register(NRF_STATUS, &reg, sizeof(reg));

	//read fifo
	if((reg & 0xe) != 0xe) {
		uint8_t p[4];
		uint16_t key, presses;
		nrf_read_rx_payload(p, sizeof(p));
		key = (*(uint16_t*)p) & 0xff;
		presses = *(uint16_t*)(p+2);

		(void)presses;

		//bottom
		if(key == 0x1e) {
			gpio_clear(PORT_LED, PIN_LED_R);
			gpio_clear(PORT_IO_0, PIN_IO_0);
		} else if(key == 0xf) { //middle
			gpio_set(PORT_LED, PIN_LED_R);
			gpio_set(PORT_IO_0, PIN_IO_0);
		}

	}

	return reg & 64;
}

static void testPipeRxCallback(nrfPipeHandle_t handle, const void* rxData, uint8_t len)
{
	const uint8_t *packet = (const uint8_t *)rxData;
	const uint8_t blubb1 = packet[0];
	const uint8_t blubb2 = packet[1];
	if (2 == len)
	{
		UNUSED(blubb1);
		UNUSED(blubb2);
		if(blubb1 == 0x1e) {
				gpio_clear(PORT_LED, PIN_LED_G);
				gpio_clear(PORT_IO_0, PIN_IO_0);
		} else if(blubb1 == 0xf) { //middle
				gpio_set(PORT_LED, PIN_LED_G);
				gpio_set(PORT_IO_0, PIN_IO_0);
		}

	}
	UNUSED(handle);
}

static void blubb(void);
MODULE_INIT_FUNCTION(blubb, 9, blubb);
static void blubb(void)
{
	int baseAddr = 0xe7e7;
	nrf_setRxBaseAddress(&baseAddr, 2);
	nrf_setupNewRxDataPipe(0xe7, 2, &testPipeRxCallback, false);

	gpio_mode_setup(PORT_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED_G);
	gpio_clear(PORT_LED, PIN_LED_G);

//	const char packet[] = "Hallo Welt";
//	nrf_sendPacket(&baseAddr, sizeof(baseAddr), packet, sizeof(packet), true, 0);
//	nrf_test();
}
