/*
 * UART.c
 *
 * Created: 4/22/2026 4:13:20 PM
 *  Author: Josue
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"

void init_UART()
{
	// CONFIGURAR PINES RX (PD0) Y TX (PD1)
	DDRD	&=	~(1 << DDD0);
	DDRD	|=	(1 << DDD1);
	
	UCSR0A	= 0;
	// HABILITANDO INTERRUPCION DE RX, HABILITANDO RX Y TX DEL UART0
	UCSR0B	= 0;
	UCSR0B	= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	// HABILITANDO MODO ASYNCRONO, PARIDA DESHABILITADA, 1 STOP BIT, 8 DATA BITS
	UCSR0C	= 0;
	UCSR0C	= (1 << UCSZ01) | (1 << UCSZ00);
	// SETEAR UBRR0
	UBRR0	= 103;
}
void writeChar(char caracter)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0	= caracter;
}
void writeString(char* string)
{
	for (uint8_t i=0; string[i] != '\0'; i++)
	{
		writeChar(string[i]);
	}
}