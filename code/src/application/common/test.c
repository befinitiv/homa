/*
 * test.c
 *
 *  Created on: Oct 25, 2012
 *      Author: lutz
 */


#include <flawless/init/systemInitializer.h>
#include <flawless/core/msg_msgPump.h>
#include <flawless/config/msgIDs.h>

#include <libopencm3/stm32/f0/rcc.h>

#include "libopencm3/stm32/gpio.h"

static void test_init(void);
MODULE_INIT_FUNCTION(test,9, test_init);
static void test_init(void)
{
	RCC_AHBENR |= RCC_AHBENR_GPIOAEN;
	
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);

	gpio_set(GPIOA, GPIO6);

}

