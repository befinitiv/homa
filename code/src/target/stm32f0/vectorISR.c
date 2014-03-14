/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>,
 * Copyright (C) 2012 chrysn <chrysn@fsfe.org>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/cm3/vector.h>

extern unsigned int _stack;

#pragma weak wwdg_isr = blocking_handler
#pragma weak pvd_isr = blocking_handler
#pragma weak rtc_isr = blocking_handler
#pragma weak flash_isr = blocking_handler
#pragma weak rcc_isr = blocking_handler
#pragma weak exti0_1_isr = blocking_handler
#pragma weak exti2_3_isr = blocking_handler
#pragma weak exti4_15_isr = blocking_handler
#pragma weak tsc_isr = blocking_handler
#pragma weak dma1_channel1_isr = blocking_handler
#pragma weak dma1_channel2_3_isr = blocking_handler
#pragma weak dma1_channel4_5_isr = blocking_handler
#pragma weak adc_comp_isr = blocking_handler
#pragma weak tim1_brk_up_trg_com_isr = blocking_handler
#pragma weak tim1_cc_isr = blocking_handler
#pragma weak tim2_isr = blocking_handler
#pragma weak tim3_isr = blocking_handler
#pragma weak tim6_dac_isr = blocking_handler
#pragma weak tim7_isr = blocking_handler
#pragma weak tim14_isr = blocking_handler
#pragma weak tim15_isr = blocking_handler
#pragma weak tim16_isr = blocking_handler
#pragma weak tim17_isr = blocking_handler
#pragma weak i2c1_isr = blocking_handler
#pragma weak i2c2_isr = blocking_handler
#pragma weak spi1_isr = blocking_handler
#pragma weak spi2_isr = blocking_handler
#pragma weak usart1_isr = blocking_handler
#pragma weak usart2_isr = blocking_handler
#pragma weak usart3_4_isr = blocking_handler
#pragma weak cec_can_isr = blocking_handler
#pragma weak usb_isr = blocking_handler

/**@}*/

#define IRQ_HANDLERS \
    [NVIC_WWDG_IRQ] = wwdg_isr, \
    [NVIC_PVD_IRQ] = pvd_isr, \
    [NVIC_RTC_IRQ] = rtc_isr, \
    [NVIC_FLASH_IRQ] = flash_isr, \
    [NVIC_RCC_IRQ] = rcc_isr, \
    [NVIC_EXTI0_1_IRQ] = exti0_1_isr, \
    [NVIC_EXTI2_3_IRQ] = exti2_3_isr, \
    [NVIC_EXTI4_15_IRQ] = exti4_15_isr, \
    [NVIC_TSC_IRQ] = tsc_isr, \
    [NVIC_DMA1_CHANNEL1_IRQ] = dma1_channel1_isr, \
    [NVIC_DMA1_CHANNEL2_3_IRQ] = dma1_channel2_3_isr, \
    [NVIC_DMA1_CHANNEL4_5_IRQ] = dma1_channel4_5_isr, \
    [NVIC_ADC_COMP_IRQ] = adc_comp_isr, \
    [NVIC_TIM1_BRK_UP_TRG_COM_IRQ] = tim1_brk_up_trg_com_isr, \
    [NVIC_TIM1_CC_IRQ] = tim1_cc_isr, \
    [NVIC_TIM2_IRQ] = tim2_isr, \
    [NVIC_TIM3_IRQ] = tim3_isr, \
    [NVIC_TIM6_DAC_IRQ] = tim6_dac_isr, \
    [NVIC_TIM7_IRQ] = tim7_isr, \
    [NVIC_TIM14_IRQ] = tim14_isr, \
    [NVIC_TIM15_IRQ] = tim15_isr, \
    [NVIC_TIM16_IRQ] = tim16_isr, \
    [NVIC_TIM17_IRQ] = tim17_isr, \
    [NVIC_I2C1_IRQ] = i2c1_isr, \
    [NVIC_I2C2_IRQ] = i2c2_isr, \
    [NVIC_SPI1_IRQ] = spi1_isr, \
    [NVIC_SPI2_IRQ] = spi2_isr, \
    [NVIC_USART1_IRQ] = usart1_isr, \
    [NVIC_USART2_IRQ] = usart2_isr, \
    [NVIC_USART3_4_IRQ] = usart3_4_isr, \
    [NVIC_CEC_CAN_IRQ] = cec_can_isr, \
    [NVIC_USB_IRQ] = usb_isr


void main(void);
void blocking_handler(void);
void null_handler(void);

__attribute__ ((section(".vectors")))
vector_table_t vector_table = {
	.initial_sp_value = &_stack,
	.reset = reset_handler,
	.nmi = nmi_handler,
	.hard_fault = hard_fault_handler,

	.sv_call = sv_call_handler,
	.pend_sv = pend_sv_handler,
	.systick = sys_tick_handler,
	.irq = {
		IRQ_HANDLERS
	}
};

void __attribute__ ((naked)) reset_handler(void)
{
	/* Call the application's entry point. */
	main();
}

void blocking_handler(void)
{
	while (1);
}

void null_handler(void)
{
	/* Do nothing. */
}

#pragma weak nmi_handler = null_handler
#pragma weak hard_fault_handler = blocking_handler
#pragma weak sv_call_handler = null_handler
#pragma weak pend_sv_handler = null_handler
#pragma weak sys_tick_handler = null_handler

/* Those are defined only on CM3 or CM4 */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
#pragma weak mem_manage_handler = blocking_handler
#pragma weak bus_fault_handler = blocking_handler
#pragma weak usage_fault_handler = blocking_handler
#pragma weak debug_monitor_handler = null_handler
#endif

