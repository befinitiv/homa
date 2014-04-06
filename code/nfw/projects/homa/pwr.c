#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencmsis/core_cm3.h>


#include "nrf.h"



void pwr_enter_standby(void) {
	nrf_pwr_down();

	rcc_periph_clock_enable(RCC_PWR);
	PWR_CR |= PWR_CR_CWUF | PWR_CR_PDDS | PWR_CR_LPDS;



	SCB_SCR |= SCB_SCR_SLEEPDEEP;

	asm volatile ("wfe");
}


//TODO: test me!
void pwr_enter_stop(void) {
	nrf_pwr_down();

	rcc_periph_clock_enable(RCC_PWR);
	PWR_CR |= PWR_CR_CWUF | PWR_CR_LPDS;
	PWR_CR &= ~PWR_CR_PDDS;




	SCB_SCR |= SCB_SCR_SLEEPDEEP;

	asm volatile ("wfe");
}
