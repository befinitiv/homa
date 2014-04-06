#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/pwr.h>




void rtc_set_time(uint8_t pm, uint8_t hour_ten, uint8_t hour_unit, uint8_t minute_ten, uint8_t minute_unit, uint8_t second_ten, uint8_t second_unit) {
	RTC_TR = (hour_ten << RTC_TR_HT_SHIFT) | (hour_unit << RTC_TR_HU_SHIFT) | (minute_ten << RTC_TR_MNT_SHIFT) | (minute_unit << RTC_TR_MNU_SHIFT) | (second_ten << RTC_TR_ST_SHIFT) | (second_unit << RTC_TR_SU_SHIFT);
	if(pm)
		RTC_TR |= RTC_TR_PM;
}

void rtc_set_date(uint8_t year_ten, uint8_t year_unit, uint8_t week_day, uint8_t month_ten, uint8_t month_unit, uint8_t date_ten, uint8_t date_unit) {
	RTC_DR = (year_ten << RTC_DR_YT_SHIFT) | (year_unit << RTC_DR_YU_SHIFT) | (week_day << RTC_DR_WDU_SHIFT) | (month_ten << RTC_DR_MT_SHIFT) | (month_unit << RTC_DR_MU_SHIFT) | (date_ten << RTC_DR_DT_SHIFT) | (date_unit << RTC_DR_DU_SHIFT);
}





void rtc_enter_init_mode(void) {
	//enable initialization mode for setting date and time
	RTC_ISR |= RTC_ISR_INIT;

	//wait until we are in init mode
	while (!(RTC_ISR & RTC_ISR_INITF));
}


void rtc_exit_init_mode(void) {
	//exit init mode.
	RTC_ISR &= ~RTC_ISR_INIT;
}



//TODO: test me!
void rtc_enable_periodic_wakeup(uint8_t coarse, uint16_t wakeup_time) {
	rtc_unlock();

	//disable WUT
	RTC_CR &= ~RTC_CR_WUTE;

	//wait until we can access WUT[15:0] and WUCLKSEL
	while(!(RTC_ISR & RTC_ISR_WUTWF));

	RTC_WUTR = wakeup_time;

	if(coarse)
		RTC_CR |= RTC_CR_WUCLKSEL_SPRE; //clock the WUT with 1Hz [1s - 32d]
	else
		RTC_CR |= RTC_CR_WUCLKSEL_RTC_DIV16; //clock with 32768Hz/16 == 2048Hz [488us - 32s]



	//enable WUT
	RTC_CR |= RTC_CR_WUTE;

	int rtc_cr = RTC_CR;

	rtc_lock();
}

void rtc_init_hardware(uint8_t use_internal_lsi_osc) {
	uint32_t t;

	//disable RCC_BDCR write protection
	rcc_periph_clock_enable(RCC_PWR);
	PWR_CR |= PWR_CR_DBP;

	if(use_internal_lsi_osc) {
		//enable internal 32k rc oscillator (not needed if external is used)
		RCC_CSR |= RCC_CSR_LSION;
	}

	//reset rtc domain
	RCC_BDCR |= RCC_BDCR_BDRST;
	RCC_BDCR &= ~RCC_BDCR_BDRST;

	if(use_internal_lsi_osc) {
		//enable rtc and the oscillators
		RCC_BDCR |= RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSI;
	}
	else {
		//enable rtc and the oscillators
		RCC_BDCR |= RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE | RCC_BDCR_LSEDRV_LOW | RCC_BDCR_LSEON;
	}

	//disable clock for power management
	rcc_periph_clock_disable(RCC_PWR);


	rtc_unlock();
	rtc_enter_init_mode();

	rtc_set_time(0, 0, 8, 5, 0, 0, 0);
	rtc_set_date(1, 4, 6, 0, 3, 2, 9);
	RTC_CR |= RTC_CR_FMT;
	RTC_CR |= RTC_CR_WUCLKSEL_SPRE;
	RTC_CR |= RTC_CR_WUTE;

	rtc_exit_init_mode();
	rtc_lock();

}


void rtc_init(void) {
	rtc_init_hardware(0);

	rtc_enable_periodic_wakeup(0, 40000);
}


void rtc_test(void) {
	int s = (RTC_TR & RTC_TR_SU_MASK) + ((RTC_TR >> RTC_TR_ST_SHIFT) & RTC_TR_ST_MASK ) * 10;
	int m = ((RTC_TR >> RTC_TR_MNU_SHIFT) & RTC_TR_MNU_MASK) + ((RTC_TR >> RTC_TR_MNT_SHIFT) & RTC_TR_MNT_MASK) * 10;
	int wut = RTC_WUTR;


	int wut_isr = RTC_ISR;
	RTC_ISR &= ~RTC_ISR_WUTF;

	int wut_cr = RTC_CR;

}
