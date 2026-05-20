/*
 * EEPROM.c
 *
 * Created: 4/15/2026 5:15:03 PM
 *  Author: Josue
 */ 
/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART_LIB/UART.h"
#include "EEPROM/EEPROM.h"
/****************************************/
// Function prototypes
void check_L1();
void check_L2();

const char*	L1_on	= "L1:1";
const char* L1_off	= "L1:0";
const char*	L2_on	= "L2:1";
const char* L2_off	= "L2:0";

char* comando		= "----";
uint8_t num_receive = 0;
uint8_t dir_eeprom = 0;


/****************************************/
// Main Function
int main(void)
{
	cli();
	// PD5 Y PD6 COMO SALIDAS INICIALMENTE APAGADAS
	DDRD	|= (1<<DDD6) | (1<<DDD5);
	PORTD	&= ~((1<<DDD5) | (1<<DDD6));
	// PD2 COMO ENTRADA CON PULL UPS HABILITADOS
	DDRD	&= ~(1<<DDD2);
	PORTD	|= (1<<DDD2);
	// HABILITAR INTERRUPCIONES DE PORTD
	PCICR	|= (1<<PCIE2);	// HABILITO INTS PARA PORTD
	PCMSK2	|= (1<<PCINT18); // HABILITO INTS PARA EL PD2
	init_UART();
	sei();
	
	uint8_t lectura = eepromRead(dir_eeprom);
	while (lectura != 0xFF)
	{
		writeChar(lectura);
		dir_eeprom++;
		lectura = eepromRead(dir_eeprom);
	}
	while(1)
	{
		//TODO:: Please write your application code
	}
}
/****************************************/
// NON-Interrupt subroutines
void check_L1()
{
	if (*(comando+0) == *(L1_on+0) &&
	*(comando+1) == *(L1_on+1) &&
	*(comando+2) == *(L1_on+2))
	{
		if (*(comando) == *(L1_on+3))
		{
			PORTD |= (1<<PORTD5);
		}else if (*(comando) == *(L1_off+3))
		{
			PORTD &= ~(1<<PORTD5);
		}
	}
}
void check_L2()
{
	if (*(comando+0) == *(L2_on+0) &&
	*(comando+1) == *(L2_on+1) &&
	*(comando+2) == *(L2_on+2))
	{
		if (*(comando) == *(L2_on+3))
		{
			PORTD |= (1<<PORTD6);
		}else if (*(comando) == *(L2_off+3))
		{
			PORTD &= ~(1<<PORTD6);
		}
	}
}
/****************************************/
// Interrupt routines
ISR(USART_RX_vect)
{
	char bufferRX = UDR0;
	
	if (bufferRX != '\n')
	{
		*(comando+num_receive) = bufferRX;
		eepromWrite(dir_eeprom, bufferRX);
		check_L1();
		check_L2();
		num_receive++;
	}else{
		for (uint8_t i = 0; i<4; i++)
		{
			writeChar(*(comando+num_receive));
			*(comando+i) = '-';
		}
		num_receive = 0;
	}
}
ISR(PCINT2_vect)
{
	uint8_t estadoPD2 = PIND & (1<<PIND2);
	if (estadoPD2 != (1<<PIND2))
	{
		PORTD ^= (1<<PORTD5);
	}
}


/*
// POSICION GUARDADO 1
ADC0    0x0000
ADC1    0x0001
ADC2    0x0002
ADC3    0x0003
ADC4    0x0004
ADC5    0x0005

// POSICION GUARDADO 2   
ADC0    0x0006
ADC1    0x0007
ADC2    0x0008
ADC3    0x0009
ADC4    0x000A
ADC5    0x000B
ADC6    0x000C
*/


