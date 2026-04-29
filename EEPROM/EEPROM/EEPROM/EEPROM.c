/*
 * EEPROM.c
 *
 * Created: 4/22/2026 5:42:58 PM
 *  Author: Josue
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "EEPROM.h"

void eepromWrite(uint16_t direccion, uint8_t dato)
{
	// ESPERAR A QUE SE ESCRIBA EL DATO ANTERIOR
	while(EECR & (1<<EEPE));
	// SETEAR DIRECCION A ESCRIBIR Y DATO A ESCRIBIR
	EEAR	= direccion;
	EEDR	= dato;
	// MASTER WRITE ENABLE
	EECR	|= (1<<EEMPE);
	// EN MENOS DE 4 CICLOS DE RELOJ, DEBO DE SETEAR EEPE PARA ESCRIBIR
	EECR	|= (1<<EEPE);
}
uint8_t eepromRead(uint16_t direccion)
{
	// ESPERAR A QUE SE ESCRIBA EL DATO ANTERIOR
	while(EECR & (1<<EEPE));
	// SETEAR DIRECCION A ESCRIBIR Y DATO A ESCRIBIR
	EEAR	= direccion;
	// MASTER WRITE ENABLE
	EECR	|= (1<<EEMPE);
	// EN MENOS DE 4 CICLOS DE RELOJ, DEBO DE SETEAR EEPE PARA ESCRIBIR
	return	EEDR;
}