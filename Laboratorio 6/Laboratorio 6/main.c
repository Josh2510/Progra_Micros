/*
 * Laboratorio 6.c
 *
 * Created: 4/24/2026 8:23:15 AM
 * Author : Josue
 */ 

/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "UART/UART.h"

/****************************************/
// Function prototypes
void Menu();
void init_ADC();

volatile uint8_t Valor_ADC = 0;
uint8_t ASCII = 0;
uint8_t nibbleH = 0;
uint8_t nibbleL = 0;
char respuesta = '0';
/****************************************/
// Main Function
int main(void)
{
	cli();
	// HABILITAR SALIDAS
	// PORT B INICIALMENTE APAGADOS DE PB0 A PB4
	DDRB	|= ((1 << DDB4) | (1 << DDB3) | (1 << DDB2) | (1 << DDB1) | (1 << DDB0));
	PORTB	&= ~((1 << PORTB4) | (1 << PORTB3) | (1 << PORTB2) | (1 << PORTB1) | (1 << PORTB0));
	// PORTD INICIALMENTE APAGADO DE PD5 A PD7
	DDRD	|= ((1 << DDD7) | (1 << DDD6) | (1 << DDD5));
	PORTD	&= ~((1 << PORTD7) | (1 << PORTD6) | (1 << PORTD5));
	// INICIALIZAR MODULO UART Y LECTURA DE ADC0
	init_UART();
	init_ADC();
	sei();
	Menu();
	while (1)
	{
		
	}
}
/****************************************/
// NON-Interrupt subroutines
void Menu()
{
	writeString("Elija Una Opcion\n");
	writeString(" 1- Medir Potenciometro\n");
	writeString(" 2- Enviar ASCII\n");
}
void init_ADC()
{
	// HABILITAR ADC CON VOLTAJE EXTERNO Y EN EL ADC0
	ADMUX = 0;
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	// HABILITAR ADC, SU INTERRUPCION Y PONER PRESCALER DE 8
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN) | (1<<ADIE)| (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADCSRA |= (1 << ADSC);
}
/****************************************/
// Interrupt routines
ISR(USART_RX_vect)
{
	if (ASCII)
	{
		uint8_t bufferRX = UDR0;
		writeChar(bufferRX);
		writeChar('\n');
		PORTD = (bufferRX << 5);
		PORTB = (bufferRX >> 3);
		ASCII = 0;
		Menu();
	}else
	{
		respuesta = UDR0;
		if (respuesta == '1')
		{
			Valor_ADC = 1;
			respuesta = 0;
			writeString("Potenciometro");
			writeChar('\n');
		}else if (respuesta == '2')
		{
			ASCII = 1;
			respuesta = 0;
			writeString("Envie el caracter\n");
		}else
		{
			writeString("Envie una respuesta valida\n");
			Menu();
		}
	}
}
ISR(ADC_vect)
{
	if (Valor_ADC)
	{
		PORTD = (ADCH << 5);
		PORTB = (ADCH >> 3);
		char BUFFER[20];
		sprintf(BUFFER, "Valor ADC: %u\n", ADCH);
		writeString(BUFFER);
		writeChar('\n');
		Menu();
		Valor_ADC = 0;
	}
	ADCSRA |= (1 << ADSC);
}