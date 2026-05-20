/*
 * Prueba.c
 *
 * Created: 5/8/2026 4:50:55 PM
 * Author : Josue
 */ 


#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include "P/PWM0.h"

// Function prototypes
void setup();
void ADC_init();

volatile uint16_t LED = 0;

int main(void)
{
    setup();
    while (1) 
    {
		UPDATE_Duty_Cycle_PWM0A(LED);
    }
}

void setup()
{
	DDRD = (1 << DDD6) | (1 << DDD5) | (1 << DDD3);
	DDRB = (1 << DDB3) | (1 << DDB2) | (1 << DDB1);
	PORTD = (1 << PORTD6) | (1 << PORTD5) | (1 << PORTD3);
	PORTB = (1 << PORTB3) | (1 << PORTB2) | (1 << PORTB1);
}
void ADC_init()
{
	// HABILITAR ADC CON VOLTAJE EXTERNO Y EN EL ADC0
	ADMUX = 0;
	ADMUX |= (1 << REFS0) | (1 << MUX1) | (1 << MUX0);
	// HABILITAR ADC, SU INTERRUPCION Y PONER PRESCALER DE 8
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN) | (1<<ADIE)| (1<<ADPS1) | (1<<ADPS0);
	ADCSRA |= (1 << ADSC);
}
ISR(ADC_vect)
{
	LED = ADC;
	ADCSRA |= (1 << ADSC);
}