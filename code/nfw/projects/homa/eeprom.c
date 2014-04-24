/*
 * eeprom.c
 *
 *  Created on: 15.04.2014
 *      Author: renate
 */

#include <libopencm3/stm32/flash.h>


#define EEPROM_START 0x0800FC00
#define EEPROM_LENGTH 256 //in 32bit words


//TODO: not yet working. even though the flash gets written correctly, the variables keep their init value. looking at the assembler could help here
//example variable: __attribute__((__section__(".eeprom"))) volatile static const int variable_name;
void eeprom_write(void *addr, void *data, int len) {
	uint8_t t[EEPROM_LENGTH]; //quite a high ram usage here. this could be avoided by using a second page of flash as buffer

	//save the old content
	memcpy(t, (void*)EEPROM_START, EEPROM_LENGTH);
	//overwrite the changed content
	memcpy(t+((uint32_t)addr - EEPROM_START), data, len);

	flash_unlock();
	flash_erase_page(EEPROM_START);


	//write the content back
	int i;
	for(i=0; i<EEPROM_LENGTH; i+=4)
		flash_program_word(EEPROM_START + i, *(uint32_t*)(t+i));

	memcpy(t, (void*)EEPROM_START, EEPROM_LENGTH);

	flash_lock();
}
