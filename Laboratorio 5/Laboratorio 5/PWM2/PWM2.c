/*
 * PWM2.c
 *
 * Created: 4/17/2026 5:03:07 PM
 *  Author: Josue
 */ 

#include <avr/io.h>
#include "PWM2.h"

void PWM2_init()
{
	// ACTIVAR OC2A (PB3)
	DDRB	|=	(1 << DDB3);
	
	// CONFIGURAR MODO FAST PWM EN MODO 3
	TCCR2A	=	(1 << COM2A1) | (1 << WGM21) | (1 << WGM20);	// NO INVERTIDO
	TCCR2B	=	(1 << CS22) | (1 << CS21) | (1 << CS20);		// PRESCALER DE 1024
	
	OCR2A	=	0;
}
void UPDATE_Duty_Cycle_PWM2(uint8_t dutyCicle)
{
	OCR2A	= dutyCicle;
}