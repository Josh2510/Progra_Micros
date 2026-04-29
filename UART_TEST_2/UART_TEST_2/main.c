/*
 * main.c
 *
 * Created: 4/15/2026 5:15:03 PM
 *  Author: Josue
 */ 
/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART_LIB/UART.h"
/****************************************/
// Function prototypes

/****************************************/
// Main Function
int main(void)
{
	cli();
	DDRD	|= (1<<DDD5);
	PORTD	&= ~(1<<DDD5);
	init_UART();
	sei();
	writeChar('H');
	writeChar('O');
	writeChar('L');
	writeChar('A');
	
	writeString("SECCION");
	while(1)
	{
		//TODO:: Please write your application code
	}
}
/****************************************/
// NON-Interrupt subroutines

/****************************************/
// Interrupt routines
ISR(USART_RX_vect)
{
	char bufferRX = UDR0;
	writeChar(bufferRX);
	if (bufferRX == 'a')
	{
		PORTD ^= (1<<PORTD5);
	}
}