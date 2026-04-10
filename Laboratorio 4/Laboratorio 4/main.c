/*
 * main.c
 *
 * Created: 4/10/2026 9:57:57 AM
 *  Author: Josue
 */ 
/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>

/****************************************/
// Variables globales
volatile uint8_t contador = 0;

/****************************************/
// Function prototypes
void	setup();
/****************************************/
// Main Function
int main(void)
{
	setup();
	while(1)
	{
		PORTD = contador;
	}
}

/****************************************/
// NON-Interrupt subroutines
void setup()
{
	cli();
	// Configurar PORTS
	// PORTC ENTRADA CON PULL UPS
	DDRC	&=	~((1 << PORTC0) | (1 << PORTC1));
	PORTC	|=	 ((1 << PORTC0) | (1 << PORTC1));
	
	// PORT D SALIDA APAGADO
	DDRD	=	0xFF;	
	PORTD	=	0x00;
	
	// HABILITAR INTERRUPCIONES DE PORTC
	PCICR	|= (1 << PCIE1);
	PCMSK1	|= ((1 << PCINT8) | (1 << PCINT9));	
	sei();
}
/****************************************/
// Interrupt routines
ISR(PCINT1_vect)
{
	if (!(PINC & 0b00000001))
	{
		contador++;
	}
	if (!(PINC & 0b00000010))
	{
		contador--;
	}
}