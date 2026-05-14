/*
 * Proyecto_Wall-e.c
 *
 * Created: 5/4/2026 4:36:24 PM
 * Author : Josue
 */ 

/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include "EEPROM/EEPROM.h"
#include "PWM BRAZOS/PWM0.h"
#include "PWM OJOS/PWM1.h"
#include "PWM DC/PWM2.h"
#include "UART/UART.h"

/****************************************/
// Function prototypes
void setup();
void ADC_init();

/****************************************/
// Variables

volatile uint16_t contador_ADC = 0;
volatile uint16_t ojo_izquierdo = 0;
volatile uint16_t ojo_derecho = 0;
volatile uint16_t brazo_izquierdo = 0;
volatile uint16_t brazo_derecho = 0;
volatile uint8_t rueda_izquierda = 0;
volatile uint8_t rueda_derecha = 0;
/****************************************/
// Main Function
int main(void)
{
	setup();
	while (1)
	{
		UPDATE_Duty_Cycle_PWM0A(6 + (brazo_derecho * 31) / 1023);
		UPDATE_Duty_Cycle_PWM0B(6 + (brazo_izquierdo * 31) / 1023);
		UPDATE_Duty_Cycle_PWM1A(800 + ((float)ojo_derecho * 4000.0) / 1023.0);
		UPDATE_Duty_Cycle_PWM1B(800 + ((float)ojo_izquierdo * 4000.0) / 1023.0);
		UPDATE_Duty_Cycle_PWM2A(rueda_derecha);
		UPDATE_Duty_Cycle_PWM2B(rueda_izquierda);
	}
}
/****************************************/
// NON-Interrupt subroutines
void setup()
{
	// DESHABILITAR INTERRUPCIONES GLOBALES
	cli();
	// HABILITAMOS PINES PARA MODULO PUENTE H
	DDRD	|=	(1 << DDD7);
	DDRB	|=	(1 << DDB0);
	PORTD   |=	(0 << PORTD7);
	PORTB   |=	(1 << PORTB0);
	// LLAMAR INICIALIZAIONES
	PWM0_init();
	PWM1_init();
	PWM2_init();
	init_UART();
	ADC_init();
	// HABILITAR INTERRUPCIONES GLOBALES
	sei();
}
void ADC_init()
{
	// HABILITAR ADC CON VOLTAJE EXTERNO Y EN EL ADC0
	ADMUX = 0;
	ADMUX |= (1 << REFS0);
	// HABILITAR ADC, SU INTERRUPCION Y PONER PRESCALER DE 32
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS0);
	ADCSRA |= (1 << ADSC);
}
/****************************************/
// Interrupt routines
ISR(ADC_vect)
{
	switch (contador_ADC)
	{
		case 0:
			brazo_derecho = ADC;
			ADMUX = (1 << REFS0) | (1 << MUX0);
			break;
		case 1:
			brazo_izquierdo = ADC;
			ADMUX = (1 << REFS0) | (1 << MUX1);
			break;
		case 2:
			ojo_derecho	= ADC;
			ADMUX = (1 << REFS0) | (1 << MUX1) | (1 << MUX0);
			break;
		case 3:
			ojo_izquierdo = ADC;
			ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX2);
			break;
		case 4:
			rueda_derecha = ADCH;
			ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX2) | (1 << MUX0);
			break;
		case 5:
			rueda_izquierda	= ADCH;
			ADMUX = (1 << REFS0);
			break;
		default:
		contador_ADC = 0x00;
		ADMUX = (1 << REFS0);
		break;
	}
	contador_ADC++;
	contador_ADC %= 6;
	ADCSRA |= (1 << ADSC);
}