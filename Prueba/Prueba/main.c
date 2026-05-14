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
	ADC_init();
	PWM0_init();
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