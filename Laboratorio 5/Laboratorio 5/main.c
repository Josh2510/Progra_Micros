/*
 * Laboratorio 5.c
 *
 * Created: 4/16/2026 10:19:40 PM
 * Author : Josue
 */ 
/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "PWM0/PWM0.h"
#include "PWM1/PWM1.h"
#include "PWM2/PWM2.h"

/****************************************/
// Function prototypes
void setup();
void ADC_init();
void TMR0_init();

volatile uint16_t servo1 = 0;
volatile uint16_t servo2 = 0;
volatile uint16_t led = 0;
volatile uint16_t contador_ADC = 0;
/****************************************/
// Main Function
int main(void)
{
	setup();	
	while (1)
	{
		UPDATE_Duty_Cycle_PWM1(800 + (servo1 * 4000)/1023);
		UPDATE_Duty_Cycle_PWM2(6 + (servo2 * 31)/1023);
		UPDATE_Duty_Cycle_PWM0(led>>2);
	}
}
/****************************************/
// NON-Interrupt subroutines
void setup()
{
	cli();
	//	F_sistema = 16 MHz
	CLKPR =	(1<<CLKPCE);
	// LLAMADO A INICIALIZACIONES
	PWM0_init();
	PWM1_init();
	PWM2_init();
	ADC_init();
	sei();
}
void ADC_init()
{
	// HABILITAR ADC CON VOLTAJE EXTERNO Y EN EL ADC0
	ADMUX = 0;
	ADMUX |= (1 << REFS0);
	// HABILITAR ADC, SU INTERRUPCION Y PONER PRESCALER DE 8
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN) | (1<<ADIE)| (1<<ADPS1) | (1<<ADPS0);
	ADCSRA |= (1 << ADSC);
}
/****************************************/
// Interrupt routines
ISR(ADC_vect)
{
	switch (contador_ADC)
	{
		case 0:
			servo1 = ADC;
			ADMUX = (1 << REFS0) | (1 << MUX0);
			break;
		case 1:
			servo2 = ADC;
			ADMUX = (1 << REFS0) | (1 << MUX1);
			break;
		case 2:
			led	= ADC;
			ADMUX = (1 << REFS0);
			break;
		default:
			contador_ADC = 0x00;
			ADMUX = (1 << REFS0);
		break;
	}
	contador_ADC++;
	contador_ADC %= 3;
	ADCSRA |= (1 << ADSC);
}