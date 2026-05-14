/*
 * PWM0.c
 *
 * Created: 4/17/2026 5:00:22 PM
 *  Author: Josue
 */ 

#include <avr/io.h>
#include "PWM0.h"

void PWM0_init()
{
	// ACTIVAR SALIDA
	DDRD	|=	(1 << DDD6) | (1 << DDD5);
	
	// CONFIGURAR MODO FAST PWM EN MODO 3
	TCCR0A =	(1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);	// NO INVERTIDO
	TCCR0B =	(1 << CS02) | (1 << CS00);						// PRESCALER DE 1024
	
	OCR0A	=	0;
	OCR0B	=	0;
}
void UPDATE_Duty_Cycle_PWM0A(uint8_t dutyCicle)
{
	OCR0A = dutyCicle;
}
void UPDATE_Duty_Cycle_PWM0B(uint8_t dutyCicle)
{
	OCR0B = dutyCicle;
}
