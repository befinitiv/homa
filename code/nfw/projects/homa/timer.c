#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencmsis/core_cm3.h>




#define TIM_CLK 8e6
#define PRESCALER 8







void timer_sleep_us(int us) {
	rcc_periph_clock_enable(RCC_TIM6);
	
	TIM6_PSC = PRESCALER-1;
	TIM6_CNT = 0;
	TIM6_ARR = us;


	TIM6_SR = 0;
	TIM6_EGR = TIM_EGR_UG;
	TIM6_DIER = TIM_DIER_UIE;
	TIM6_CR1 = TIM_CR1_OPM | TIM_CR1_URS | TIM_CR1_CEN;

//	while(!(TIM6_SR & TIM_SR_UIF)) {

#define SCB_SCR_SEVONPEND (1 << 4)
	SCB_SCR |= SCB_SCR_SEVONPEND;

	asm volatile ("wfe");
//	}

	rcc_periph_clock_disable(RCC_TIM6);
	
}
