/*
* NombreProgra.asm
*
* Creado: 
* Autor : 
* Descripción: 
*/
/****************************************/
// Encabezado (Definición de Registros, Variables y Constantes)
.include "M328PDEF.inc"     // Include definitions specific to ATMega328P
.dseg
.org    SRAM_START
//variable_name:     .byte   1   // Memory alocation for variable_name:     .byte   (byte size)

.cseg
.org 0x0000
	JMP	START
.org OVF0addr
	JMP	ISR_TMR0_OVERFLOW
 /****************************************/
// Configuración de la pila
START:
	LDI     R16, LOW(RAMEND)
	OUT     SPL, R16
	LDI     R16, HIGH(RAMEND)
	OUT     SPH, R16
/****************************************/
// Configuracion MCU
SETUP:
    /************* PRESCALER **************/
	// 
	LDI		R16,	(1	<<	CLKPCE)
	STS		CLKPR,	R16			// Enable PRESCALER (must be done before setting CLKPS0-3)
	LDI		R16,	0b00000100
	STS		CLKPR,	R16			// SET PRESCALER TO 16 F_cpu = 1MHz

	// CONFIGURAR ENTRADAS / SALIDAS
	CBI		DDRD,	DDD2
	SBI		PORTD,	PORTD2

	SBI		DDRB,	DDB0
	SBI		DDRB,	DDB5
	CBI		PORTB,	PORTB0
	CBI		PORTB,	PORTB5

	LDI		R16,	0x00
	OUT		TCCR0A,	R16
	LDI		R16,	(1 << CS01) | (1 << CS00)
	OUT		TCCR0B,	R16
	LDI		R16, 100
	OUT		TCNT0,	R16

	// HABILITAR INTERRUPCIONES DE TMR0
	LDI		R16,	(1 << TOIE0)
	STS		TIMSK0,	R16

	SEI		// HABILITAR INTERRUPCIONES GLOBALES
/****************************************/
// Loop Infinito
MAIN_LOOP:
	CPI		R20,	50
	BRNE	MAIN_LOOP
	SBI		PINB,	PINB0
	SBI		PINB,	PINB5
    RJMP    MAIN_LOOP

/****************************************/
// NON-Interrupt subroutines

/****************************************/
// Interrupt routines
ISR_TMR0_OVERFLOW:
	PUSH	R16				// Store contents of R16 in stack
	IN		R16, SREG		// Store SREG in R16
	PUSH	R16				// Store contents of R16 in stack	

	INC		R20

	POP		R16				// Get contents from stack and store into R16
	OUT		SREG, R16		// Load previous contents into SREG
	POP		R16				// Get contents from stack and store into R16

	RETI
/****************************************/
