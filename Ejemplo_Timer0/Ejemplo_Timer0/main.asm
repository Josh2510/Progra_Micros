/*;
* Ejemplo_Timer0.asm
*
* Created: 2/18/2026
* Author : Josue
*
*/
/****************************************/
// Encabezado (Definición de Registros, Variables y Constantes)
.include "M328PDEF.inc"     // Include definitions specific to ATMega328P
.dseg
.org    SRAM_START
//variable_name:     .byte   1   // Memory alocation for variable_name:     .byte   (byte size)

.cseg
.org 0x0000
	JMP START
.org OVF1addr
	JMP	ISR_TMR1_OVF
START:
	.equ TCNT1_value = 0xCF2C
 /****************************************/
// Configuración de la pila
	LDI     R16, LOW(RAMEND)
	OUT     SPL, R16
	LDI     R16, HIGH(RAMEND)
	OUT     SPH, R16
/****************************************/
// Configuracion MCU
SETUP:
	// DESHABILITAR INTERRUPCIONES
	CLI
	/********** PRESCALER **********/
	//
	LDI		R16,	(1 << CLKPCE)
	STS		CLKPR,	R16
	LDI		R16,	0b00000100
	STS		CLKPR,	R16

	SBI		DDRB,	DDB0
	SBI		DDRB,	DDB5
	CBI		PORTB,	PORTB0
	CBI		PORTB,	PORTB5

	CALL INIT_TMR1

	LDI		R16,	(1 << TOIE1)
	STS		TIMSK1,	R16

	SEI
/****************************************/
// Loop Infinito
MAIN_LOOP:
    RJMP    MAIN_LOOP

/****************************************/
// NON-Interrupt subroutines
INIT_TMR1:
	LDI		R16,	0x00
	STS		TCCR1A,	R16
	LDI		R16,	(1 << CS11)
	STS		TCCR1B,	R16
	LDI		R16,	HIGH(TCNT1_value)
	STS		TCNT1H,	R16
	LDI		R16,	LOW(TCNT1_value)
	STS		TCNT1L,	R16
	RET
/****************************************/
// Interrupt routines
ISR_TMR1_OVF:
	PUSH	R16
	IN		R16,	SREG
	PUSH	R16

	LDI		R16,	HIGH(TCNT1_value)
	STS		TCNT1H,	R16
	LDI		R16,	LOW(TCNT1_value)
	STS		TCNT1L,	R16

	SBI		PINB,	PINB0
	SBI		PINB,	PINB5

	POP		R16
	OUT		SREG,	R16
	POP		R16
	RETI
/****************************************/
