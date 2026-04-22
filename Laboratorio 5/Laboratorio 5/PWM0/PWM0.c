/*
 * PWM0.c
 *
 * Created: 4/17/2026 5:00:22 PM
 *  Author: Josue
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "PWM0.h"

uint8_t limite = 0;
uint8_t contador_PWM = 0;

void PWM0_init()
{
	// ACTIVAR SALIDA
	DDRD	|=	(1 << DDD6);
	
	TCCR0A = 0;
	TCCR0B = (1 << CS01) | (1 << CS00);		// PRESCALER DE 64 PARA QUE SUCEDA CADA 1ms
	
	// HABILITAMOS INTERRUPCIONES DEL TIMER 0 POR OVERFLOW
	TIMSK0 = (1 << TOIE0);
	
	// VALOR CON EL QUE HACE OVERFLOW
	TCNT0 = 255;
}
void UPDATE_Duty_Cycle_PWM0(uint8_t dutyCicle)
{
	limite = dutyCicle;
}
ISR(TIMER0_OVF_vect)
{
	if (contador_PWM <= limite)
	{
		PORTD = 0;
	}
	else
	{
		PORTD = 0b01000000;
	}
	contador_PWM++;
	TCNT0 = 255;
}