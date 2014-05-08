#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencmsis/core_cm3.h>


#include <interfaces/nrf.h>



void system_wait_for_interrupt(void) {
//	nrf_pwr_down();

	RCC_APB1ENR |= RCC_APB1ENR_PWREN;
	PWR_CR |= PWR_CR_CWUF | PWR_CR_PDDS | PWR_CR_LPDS;



//	SCB_SCR |= SCB_SCR_SLEEPDEEP;

	__asm("wfi");
}


//TODO: test me!
void pwr_enter_stop(void) {
	nrf_pwr_down();

	RCC_APB1ENR |= RCC_APB1ENR_PWREN;
	PWR_CR |= PWR_CR_CWUF | PWR_CR_LPDS;
	PWR_CR &= ~PWR_CR_PDDS;


	SCB_SCR |= SCB_SCR_SLEEPDEEP;

	__asm("wfe");
}
