/*
 * hwdefines.h
 *
 *  Created on: 04.04.2014
 *      Author: renate
 */

#ifndef HWDEFINES_H_
#define HWDEFINES_H_


#include <libopencm3/stm32/gpio.h>

#define PORT_LED GPIOA
#define PIN_LED_G GPIO6
#define PIN_LED_R GPIO7

#define PORT_IO_0 GPIOB
#define PIN_IO_0 GPIO6


#define PORT_RF_CE GPIOA
#define PIN_RF_CE GPIO12
#define PORT_RF_IRQ GPIOA
#define PIN_RF_IRQ GPIO11


#endif /* HWDEFINES_H_ */
