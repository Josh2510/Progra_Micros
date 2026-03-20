/*
 * main.c
 *
 * Created: 3/18/2026 5:16:55 PM
 *  Author: Josue
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define T1Value 0x017E
//

void setup();
void initADC();
void initTMR1();

int main(void)
{
    cli();
	setup();
	initADC();
	// habilitar interrupciones
	ADCSRA |= (1<<ADSC) | (1<<ADIE);
	TIMSK1 = (1<<TOIE1);
	// deshabilitar interrupciones
	sei();
	while(1)
	{
		
	}
}

// NON-INTERRUP
void setup()
{
	//	F_sistema = 1MHz
	CLKPR =	(1<<CLKPCE);
	CLKPR =	(1<<CLKPS2);
	// configuraciones
	DDRD = 0xFF;
	PORTD = 0x00;
	UCSR0B = 0x00;
}

void initADC()
{
	ADMUX = 0;
	ADMUX |= (1<<REFS0) | (1<<ADLAR) | (1<<MUX2) | (1<<MUX1);
	ADCSRA = 0;
	ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);
}

void initTMR1()
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1<<CS11) | (1<<CS10);
}
// INTERRUPCIONES
ISR(ADC_vect)
{
	PORTD = ADCH;
}
ISR(TIMER1_OVF_vect)
{
	ADCSRA |= (1<<ADSC);
}