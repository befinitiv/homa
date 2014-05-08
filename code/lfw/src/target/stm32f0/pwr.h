/*
 * pwr.h
 *
 *  Created on: 07.04.2014
 *      Author: renate
 */

#ifndef PWR_H_
#define PWR_H_

#include <libopencm3/stm32/pwr.h>
#include <libopencmsis/core_cm3.h>

#define PWR_WAS_I_IN_STANDBY (PWR_CSR & PWR_CSR_SBF)


#endif /* PWR_H_ */
