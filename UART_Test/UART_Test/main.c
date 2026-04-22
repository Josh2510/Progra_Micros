/*
 * main.c
 *
 * Created: 4/15/2026 5:15:03 PM
 *  Author: Josue
 */ 
/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>
/****************************************/
// Function prototypes
void	init_UART();
void	writeChar(char caracter);
void	writeString(char* string);

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
void init_UART()
{
	// CONFIGURAR PINES RX (PD0) Y TX (PD1)	
	DDRD	&=	~(1 << DDD0);
	DDRD	|=	(1 << DDD1);
	
	UCSR0A	= 0;
	// HABILITANDO INTERRUPCIONES, HABILITANDO RX Y TX DEL UART0
	UCSR0B	= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	// HABILITANDO MODO ASYNCRONO, PARIDA DESHABILITADA, 1 STOP BIT, 8 DATA BITS
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