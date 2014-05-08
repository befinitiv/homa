/*
 * main.c
 *
 *  Created on: Oct 17, 2012
 *      Author: lutz
 */

#include <flawless/init/systemInitializer.h>

#include <flawless/core/msg_msgPump.h>

#include <flawless/logging/logging.h>
#include <target/stm32f0/pwr.h>

int main(void)
{
	if(!PWR_WAS_I_IN_STANDBY) {
			uint32_t i;
			//sleep a bit. this helps the voltage of the radio to settle. also you are not yet locked out of JTAG (which could happen if the chip goes immediately into standby mode)
			for (i = 0; i < 1000000; i++) { /* Wait a bit. */
					__asm__("nop");
			}
	}
	systemInitialize();

	LOG_VERBOSE_0("all intialized");

	msgPump_pumpMessage();

	return 0;
}
