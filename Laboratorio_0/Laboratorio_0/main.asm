;
; Laboratorio_0.asm
;
; Created: 1/23/2026 4:56:11 PM
; Author : Josue
;


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
// CONFIGURACION
SETUP:
	LDI R16, 0b00000001
	OUT DDRB, R16

/****************************************/
// LOOP INFINITO
MAIN:
	LDI R16, 0x00
	OUT PORTB, R16
	CALL DELAY
	LDI R16, 0x01
	OUT PORTB, R16
	CALL DELAY
	RJMP MAIN

/****************************************/
// FUNCIONES ADICIONALES
DELAY:
	LDI R17, 0xFF
	LDI R18, 0xFF
	LDI R19, 0xFF
	LDI R20, 0xFF
DELAY_1:
	DEC R17
	BRNE DELAY_1
DELAY_2:
	DEC R18
	BRNE DELAY_1
DELAY_3:
	DEC R19
	BRNE DELAY_1
	RET