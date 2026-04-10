/*
 * main.c
 *
 * Created: 4/8/2026 4:55:55 PM
 *  Author: Josue
 */ 
/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "PWM/PWM0.h"
/****************************************/
// Function prototypes

/****************************************/
// Main Function
int main(void)
{
	uint8_t dutyCycle = 0;
	CLKPR	=	(1<<CLKPCE);
	CLKPR	=	(1<<CLKPS2);
	initPWM0A(invertido,fastPWM,256);
	initPWM0B(invertido,fastPWM,256);
	while(1)
	{
		UPDATE_duty_cycle0A(dutyCycle);
		UPDATE_duty_cycle0B(dutyCycle);
		dutyCycle++;
		_delay_ms(1);
	}
}
/****************************************/
// NON-Interrupt subroutines

/****************************************/
// Interrupt routines
