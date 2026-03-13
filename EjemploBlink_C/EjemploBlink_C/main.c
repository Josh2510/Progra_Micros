/*
 * EjemploBlink_C.c
 *
 * Created: 3/11/2026 4:46:54 PM
 * Author : Josue
 */ 
/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>

/****************************************/
// Function prototypes
	void setup();
	void delay();
/****************************************/
// Main Function
int main(void)
{
    setup();/* Replace with your application code */
    while (1) 
    {
		delay();
		PORTC |= ((1<<PORTC3) | (1<<PORTC2) | (1<<PORTC1) | (1<<PORTC0));
		delay();
		PORTC &= ~((1<<PORTC3) | (1<<PORTC2) | (1<<PORTC1) | (1<<PORTC0));
    }
}
/****************************************/
// NON-Interrupt subroutines
void setup()
{
	CLKPR	=	(1 << CLKPCE);
	CLKPR	=	(1 << CLKPS2);
	
	DDRC	=	0xFF;
	PORTC	=	0x00;
}
void delay()
{
	for (volatile	uint8_t i = 0; i < 255; i++)
	{
		for (volatile	uint8_t j = 0; j < 255; j++)
		{
		}
	}
}
/****************************************/
// Interrupt routines