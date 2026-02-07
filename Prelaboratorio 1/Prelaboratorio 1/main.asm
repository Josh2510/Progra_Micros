/*
* Prelaboratorio 1.asm
*
* Creado: 05 / 02 / 2026
* Autor : Josue Eli Mata Ixcayau
* Descripción: Pre laboratorio 1. Contador de 4 bits con 2 push buttons y 4 leds.
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
	LDI		R16,	0x00
	OUT		DDRC,	R16		// PONIENDO EN 0 BITS 0 A 1 DE DDRC
	LDI		R16,	0xFF
	OUT		PORTC,	R16		// HABILITANDO PULL-UP PARA PC0 A PC3
	// OUTPUT ---> PB0
	LDI		R16,	0xFF
	OUT		DDRB,	R16		// PONIENDO EN 1 TODOS LOS BITS DE DDRB
	OUT		DDRD,	R16		// PONIENDO EN 1 TODOS LOS BITS DE DDRD
	LDI		R16,	0x00
	OUT		PORTB,	R16		// INICIALMENTE APAGADOS BITS DE PORTB
	OUT		PORTD,	R16		// INICIALMENTE APAGADOS BITS DE PORTB
	LDI		R16,	0x7F	// VALORES INICIALES DE PINC
	LDI		R20,	0x00	// CONTADOR_1
	LDI		R18,	0x00	// CONTADOR_1
	LDI		R22,	0x0F

/****************************************/
// Loop Infinito
MAIN_LOOP:
    IN		R17,	PINC		// LEER PINC
	CP		R16,	R17			// COMPARA VALORES INICIALES CON VALORES ACTUALES
	BREQ	MAIN_LOOP			// SI SON IGUALES ENTONCES REGRESA, SALTA HASTA QUE NO SEAN IGUALES
	CALL	DELAY
	IN		R17,	PINC		// SE VUELVE A LEER PINC
	CP		R16,	R17			// SE COMPARA VALORES OTRA VEZ PARA VER SI NO HA CAMBIADO
	BREQ	MAIN_LOOP

	MOV		R2,	R17
	EOR		R2,	R16

	SBRC	R2,	0
	CALL	BIT_0
	
	SBRC	R2,	1
	CALL	BIT_1

	SBRC	R2,	2
	CALL	BIT_2

	SBRC	R2,	3
	CALL	BIT_3

	ANDI	R20,	0x0F		// SE HACE UNA COMPARACION CON 0x0F PARA QUE NO PASE DE 4 BITS
	OR		R20,	R18
	OUT		PORTD, R20			// SE MUESTRA EN PORTD EL CONTADOR
	MOV		R16,	R17
	RJMP    MAIN_LOOP

/****************************************/
// NON-Interrupt subroutines
DELAY:
	LDI		R19, 255
	LDI		R21, 255

LOOP_DELAY_1:
	DEC		R19
	BRNE	LOOP_DELAY_1

LOOP_DELAY_2:
	DEC		R21
	BRNE	LOOP_DELAY_1
	RET

AUMENTO_C1:
	INC		R20
	RET

DISMINUYE_C1:
	DEC		R20
	RET

AUMENTO_C2:
	INC		R18
	ADD		R18,	R22
	ANDI	R18,	0xF0
	RET

DISMINUYE_C2:
	DEC		R18
	ANDI	R18,	0xF0
	RET

BIT_0:
	SBRS	R17,	 0			// SE REVISA SI EL PINC0 ESTA EN 1 PARA SALTAR LA SIGUIENTE LINEA
	CALL	AUMENTO_C1			// SI ESTA EN 0 SE INCREMENTA CONTADOR
	RET

BIT_1:
	SBRS	R17,	1			// SE REVISA SI EL PINC1 ESTA EN 1 PARA SALTAR LA SIGUIENTE LINEA
	CALL	DISMINUYE_C1		// SI ESTA EN 0 SE DECREMENTA CONTADOR
	RET

BIT_2:
	SBRS	R17,	2
	CALL	AUMENTO_C2
	RET
	
BIT_3:
	SBRS	R17,	3
	CALL	DISMINUYE_C2	
	RET
/****************************************/
// Interrupt routines

/****************************************/


