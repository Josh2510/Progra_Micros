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
 /****************************************/
// Configuración de la pila
LDI     R16, LOW(RAMEND)
OUT     SPL, R16
LDI     R16, HIGH(RAMEND)
OUT     SPH, R16
/****************************************/
// Configuracion MCU
SETUP:
    // CONFIGURAR ENTRADAS Y SALIDAS
	// INPUT ----> PD5
	CBI		DDRD, DDD5		// PONIENDO EN 0 EL BIT 5 DE DDRD
	CBI		PORTD, PORTD5	// DESHABILITANDO PULL-UP OARA PD5
	// OUTPUT ---> PB0
	SBI		DDRB, DDB0		// PONIENDO EN 1 EL BIT 0 DE DDRB
	CBI		PORTB, PORTD0	// INITIALMENTE APAGADO EL PB0

/****************************************/
// Loop Infinito
MAIN_LOOP:
    IN		R17, PIND		// Leer PIND
	ANDI	R17, 0b00100000
	BRNE	MAIN_LOOP
	CALL	DELAY
	IN		R18, PIND
	CP		R18, R17
	BRNE	MAIN_LOOP
	SBI		PINB, PINB0		// TOOGLE
	RJMP    MAIN_LOOP

/****************************************/
// NON-Interrupt subroutines
DELAY:
	LDI		R19, 255
LOOP_DELAY:
	DEC		R19
	BRNE	LOOP_DELAY
	RET
/****************************************/
// Interrupt routines

/****************************************/

