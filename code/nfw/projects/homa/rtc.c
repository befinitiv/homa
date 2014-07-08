#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/pwr.h>



#include "pwr.h"

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



void rtc_enable_periodic_alarm(uint8_t second_ten, uint8_t second_unit) {
	rtc_unlock();

	//disable ALARM
	RTC_CR &= ~RTC_CR_ALRAE;

	//wait until we can access the alarm registers
	while(!(RTC_ISR & RTC_ISR_ALRAWF));


	RTC_ALRMAR = RTC_ALRMXR_MSK4 | RTC_ALRMXR_MSK3 | RTC_ALRMXR_MSK2 | (second_ten << RTC_ALRMXR_ST_SHIFT) | (second_unit << RTC_ALRMXR_SU_SHIFT);

	//enable ALARM
	RTC_CR |= RTC_CR_ALRAE | RTC_CR_ALRAIE;


	rtc_lock();
}

void rtc_init_hardware(uint8_t use_internal_lsi_osc) {

	//disable RCC_BDCR write protection
	rcc_periph_clock_enable(RCC_PWR);
	PWR_CR |= PWR_CR_DBP;

	if(use_internal_lsi_osc) {
		//enable internal 40khz rc oscillator (not needed if external is used)
		RCC_CSR |= RCC_CSR_LSION;

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

	rtc_exit_init_mode();
	rtc_lock();

}


void rtc_init(void) {
	//TODO: this should be done only if !PWR_WAS_I_IN_STANDBY. however, without reinitializing the RTC, the alarm will not wake up the system anymore. this has to be analyzed
	rtc_init_hardware(1);

	int i;
	i = RTC_BKPXR(0);
	RTC_BKPXR(0) = ++i;
}
