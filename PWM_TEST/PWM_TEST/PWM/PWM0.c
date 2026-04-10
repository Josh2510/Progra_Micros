/*
 * PWM0.c
 *
 * Created: 4/8/2026 5:16:56 PM
 *  Author: Josue
 */ 


#include <avr/io.h>

#include "PWM0.h"

void initPWM0A(uint8_t invert, uint8_t modo, uint16_t prescaler)
{
	// OCR0A = PD6 Y OCR0B = PD5
	DDRD	|= (1<<DDD6);
	
	TCCR0A	&= ~((1<<COM0A0) | (1<<COM0A1));
	
	if (invert)
	{
		// CONFIGURAR MODO INVERTIDO O NO INVERTIDO
		TCCR0A	|= (1<<COM0A1) | (1<<COM0A0);	// INVERTIDO
	}else{
		TCCR0A	|= (1<<COM0A1);	// NO INVERTIDO
	}
	
	if (modo)
	{
		// CONFIGURAR EN MODO "PHASE PWM"
		TCCR0A	|= (1<<WGM00);
		}else{
		// CONFIGURAR EN MODO "FAST PWM"
		TCCR0A	|= (1<<WGM01) | (1<<WGM00);
	}
	
	switch(prescaler){
		case 1:
			TCCR0A |= (1<<CS00);
			break;
		case 8:
			TCCR0A |= (1<<CS01);
			break;
		case 64:
			TCCR0A |= (1<<CS01) | (1<<CS00);
			break;
		case 256:
			TCCR0A |= (1<<CS02);
			break;
		case 1024:
			TCCR0A |= (1<<CS02) | (1<<CS00);
			break;
		default:
			TCCR0A |= (1<<CS00);
	}
}
void initPWM0B(uint8_t invert, uint8_t modo, uint16_t prescaler)
{
	// OCR0A = PD6 Y OCR0B = PD5
	DDRD	|= (1<<DDD5);
	
	TCCR0B	&= ~((1<<COM0B0) | (1<<COM0B1));
	
	if (invert)
	{
		// CONFIGURAR MODO INVERTIDO O NO INVERTIDO
		TCCR0B	|= (1<<COM0B1) | (1<<COM0B0);	// INVERTIDO
		}else{
		TCCR0B	|= (1<<COM0B1);	// NO INVERTIDO
	}
	
	if (modo)
	{
		// CONFIGURAR EN MODO "PHASE PWM"
		TCCR0B	|= (1<<WGM00);
		}else{
		// CONFIGURAR EN MODO "FAST PWM"
		TCCR0B	|= (1<<WGM01) | (1<<WGM00);
	}
	
	switch(prescaler){
		case 1:
			TCCR0B |= (1<<CS00);
			break;
		case 8:
			TCCR0B |= (1<<CS01);
			break;
		case 64:
			TCCR0B |= (1<<CS01) | (1<<CS00);
			break;
		case 256:
			TCCR0B |= (1<<CS02);
			break;
		case 1024:
			TCCR0B |= (1<<CS02) | (1<<CS00);
			break;
		default:
		TCCR0B |= (1<<CS00);
	}
}
void UPDATE_duty_cycle0A(uint8_t duty)
{
	OCR0A = duty;
}
void UPDATE_duty_cycle0B(uint8_t duty)
{
	OCR0B = duty;
}