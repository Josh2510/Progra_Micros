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
#include <stdint.h>

#define TCNT0_value	131

/****************************************/
// Variables globales
volatile uint8_t contador = 0;
volatile uint8_t contador_multi = 0;
volatile uint8_t nibbleH = 0;
volatile uint8_t nibbleL = 0;
uint8_t numeros[] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10, 0x08, 0x03, 0x70, 0x21, 0x06, 0x0E};

/****************************************/
// Function prototypes
void	setup();
void	initADC();
void	init_TMR0();

/****************************************/
// Main Function
int main(void)
{
	cli();
	setup();
	initADC();
	init_TMR0();
	// HABILITAR INTERRUPCIONES DE PORTC
	PCICR	|= (1 << PCIE1);
	PCMSK1	|= ((1 << PCINT8) | (1 << PCINT9));
	// HABILITAR INTERRUPCIONES DE TIMER0
	TIMSK0	|= (1 << TOIE0);
	// HABILITAR INTERRUPCIONES DE ADC Y COMIENZA LA CONVERSION
	ADCSRA	|= (1<<ADSC) | (1<<ADIE);
	sei();
	while(1)
	{
		
	}
}

/****************************************/
// NON-Interrupt subroutines
void setup()
{
	//	F_sistema = 1MHz
	CLKPR =	(1<<CLKPCE);
	CLKPR =	(1<<CLKPS2);
	// Configurar PORTS
	// PORTC ENTRADA CON PULL UPS
	DDRC	&=	~((1 << PORTC0) | (1 << PORTC1));
	PORTC	|=	 ((1 << PORTC0) | (1 << PORTC1));
	
	DDRB = 0xff;
	PORTB = 0X00;
	
	// PORT D SALIDA APAGADO
	DDRD	=	0xFF;	
	PORTD	=	0x00;
}
void initADC()
{
	ADMUX = 0;
	ADMUX |= (1<<REFS0) | (1<<ADLAR) | (1<<MUX1);
	ADCSRA = 0;
	ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);
}
void init_TMR0()
{
	//	CONFIGURAR EN MODO NORMAL EL TIMER0
	TCCR0A	&=	~((1<<WGM01) | (1<<WGM00));
	TCCR0B	&=	~(1 << WGM02);
	//	CONFIGURAR EL PRESCALER EN 8
	TCCR0B	&=	~((1 << CS02) | (1<<CS00));
	TCCR0B	|=	(1<<CS01);
	//	INICIAR EL TCNT0
	TCNT0	=	TCNT0_value;
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
ISR(TIMER0_OVF_vect)
{
	// REINICIAR CONTADOR
	TCNT0	=	TCNT0_value;
	// MULTIPLEXADO PARA PORTB
	switch(contador_multi){
		case 0:
			PORTB	&=	~((1 << PORTB1) | (1 << PORTB2));
			PORTB	|=	(1 << PORTB0);
			PORTD	=	numeros[nibbleL];
			break;
		case 1:
			PORTB	&=	~((1 << PORTB0) | (1 << PORTB2));
			PORTB	|=	(1 << PORTB1);
			PORTD	=	numeros[nibbleH];
			break;
		case 2:
			PORTB	&=	~((1 << PORTB0) | (1 << PORTB1));
			PORTB	|=	(1 << PORTB2);
			PORTD	=	contador;
			break;
		default:
			contador_multi = 0xFF;
			break;
	}
	// AUMENTAR CONTADOR MULTIPLEXADO
	contador_multi++;
}
ISR(ADC_vect)
{
	nibbleL = (ADCH & 0x0F);
	nibbleH = (ADCH & 0xF0) >> 4;
	ADCSRA	|= (1<<ADSC);
}