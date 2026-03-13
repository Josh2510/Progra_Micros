/*
 * EjemploTMR0_C.c
 *
 * Created: 3/11/2026 5:05:14 PM
 * Author: Josue
 * Description: 
 */
/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define TCNT0_value	100
uint8_t counter = 0;

/****************************************/
// Function prototypes
void setup();
void init_TMR0();

/****************************************/
// Main Function
int main(void)
{
	cli();
	setup();
	//	HABILITAR INTERRUPCIONES DE OVERFLOW DE TIMER 0
	TIMSK0 |= (1<<TOIE0);
	sei();
	while (1)
	{
	}
}
/****************************************/
// NON-Interrupt subroutines
void setup()
{
	CLKPR	=	(1 << CLKPCE);
	CLKPR	=	(1 << CLKPS2);
	
	DDRC	=	0x0F;
	PORTC	=	0x00;
	init_TMR0();
}
void init_TMR0()
{	
	//	CONFIGURAR EN MODO NORMAL EL TIMER0
	TCCR0A	&=	~((1<<WGM01) | (1<<WGM00));
	TCCR0B	&=	~(1 << WGM02);	
	//	CONFIGURAR EL PRESCALER EN 64
	TCCR0B	&=	~(1 << CS02);
	TCCR0B	|=	((1<<CS01) | (1<<CS00));
	//	INICIAR EL TCNT0
	TCNT0	=	TCNT0_value;
}
/****************************************/
// Interrupt routines
ISR(TIMER0_OVF_vect)
{
	TCNT0	=	TCNT0_value;
	counter++;
	if (counter == 50)
	{
		counter = 0;
		PORTC++;
		PORTC &= 0x0F;
	}
}