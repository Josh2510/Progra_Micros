/*
* Laboratorio_2.asm
*
* Creado: 12 / 02 / 2026
* Autor : Josue Eli Mata Ixcayau
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
	// PRESCALER
	LDI		R16,	(1 << CLKPCE)
	STS		CLKPR,	R16				// HABILITAR CAMBIOS EN CLOCK PRINCIPAL PARA HABILITAR PRESCALER
	LDI		R16,	(1 << CLKPS2)
	STS		CLKPR,	R16				// CAMBIAR FRECUENCIA DE 16 MHz a 1MHz

	// TIMER
	LDI		R16,	(1 << CS01) | (1 << CS00) // CONFIGURANDO PRESCALER DE I/O A 64
	OUT		TCCR0B,	R16
	LDI		R16,	100				// SE GUARDA EN R16 EL VALOR DE INICIO DEL TIMER
	OUT		TCNT0,	R16				// GUARDA VALORES INICIALES DE TIMER 0 EN TCNT0

	// SALIDAS
	LDI		R16,	0xFF			
	OUT		DDRB,	R16				// PONER EN SALIDAS LOS BITS 0 A 5 DE PORTB
	LDI		R16,	0x00
	OUT		PORTB,	R16				// PONER DE VALOR INICIAL PARA PORTB 0
    
	LDI		R20,	0
/****************************************/
// Loop Infinito
MAIN_LOOP:
	IN		R16,	TIFR0		// LEE REGISTRO DE INTERRUPCIONES DE TIMER0
	SBRS	R16,	TOV0		// SALTA SI BIT DE OVERFLOW ESTA ENCENDIDA
    RJMP    MAIN_LOOP
	SBI		TIFR0,	TOV0		// REGRESA BIT DE OVERFLOW A 0, PORQUE NO SE HACE AUTOMATICAMENTE
	LDI		R16,	100			// VUELVE A CARGAR VALOR INICIAL DE TIMER
	OUT		TCNT0,	R16			// MANDA VALOR INICIAL DE TIMER
	INC		R20
	CPI		R20,	10
	BRNE	MAIN_LOOP
	CLR		R20
	CALL	AUMENTO_1			// LLAMA FUNCION DE CONTADOR
	OUT		PORTB,	R17			// SACA VALORES DE CONTADOR A PORTB
	RJMP	MAIN_LOOP

/****************************************/
// NON-Interrupt subroutines
AUMENTO_1:
	INC		R17					// INCREMENTA REGISTRO DE CONTADOR
	ANDI	R17,	0x0F		// ELIMINA LOS 4 BITS MAS SIGNIFICANTES

/****************************************/
// Interrupt routines

/****************************************/