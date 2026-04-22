/*
 * PWM1.c
 *
 * Created: 4/17/2026 9:14:35 AM
 *  Author: Josue
 */ 

#include <avr/io.h>
#include "PWM1.h"

void PWM1_init()
{
	// ACTIVAR OC1A (PB1)
	DDRB	|=	(1 << DDB1);
	
	// CONFIGURAR MODO FAST PWM EN MODO 14
	TCCR1A	=	(1 << COM1A1) |	(1	<<	WGM11);			// NO INVERTIDO
	TCCR1B	=	(1 << WGM13) | (1 << WGM12) | (1 << CS11);	// PRESCALER DE 8
	
	ICR1 = 39999;
}
void UPDATE_Duty_Cycle_PWM1(uint16_t dutyCicle)
{
	OCR1A = dutyCicle;
}