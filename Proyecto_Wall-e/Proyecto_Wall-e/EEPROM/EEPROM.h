/*
 * EEPROM.h
 *
 * Created: 4/22/2026 5:43:09 PM
 *  Author: Josue
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
#include <avr/io.h>
#include <avr/interrupt.h>

void eepromWrite(uint16_t direccion, uint8_t dato);
uint8_t eepromRead(uint16_t direccion);

#endif /* EEPROM_H_ */