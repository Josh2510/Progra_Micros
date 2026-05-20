/*
 * Proyecto_Wall-e.c
 *
 * Created: 5/4/2026 4:36:24 PM
 * Author : Josue
 * Description: Proyecto final de PROGRAMACION DE MICROCONTROLADORES para
 * programar 6 motores que se puedan mover de manera manual con el ADC,
 * con el uso de la comunicacion serial a traves de ADAFRUIT y con la
 * memoria EEPROM, guardando datos de los motores.
 */ 

/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include "EEPROM/EEPROM.h"
#include "PWM BRAZOS/PWM0.h"
#include "PWM OJOS/PWM1.h"
#include "PWM DC/PWM2.h"
#include "UART/UART.h"

/****************************************/
// Function prototypes
void setup();
void ADC_init();
void MANUAL();
void set_estado();
void ESTADO_SEGUN_UART(unsigned char estado);
void ESCRIBIR_EEPROM();
void LEER_EEPROM(uint8_t posicion);
void SETEAR_MOTOR(uint8_t n_motor, uint8_t valor);

/****************************************/
// Variables
// VARIABLES GLOBALES
volatile uint8_t estado_actual = 0;							// VARIABLE QUE INDICA EN QUE ESTADO ESTA
volatile uint8_t bandera_guardar = 0;						// VARIABLE DE BANDERA PARA INDICAR QUE ACCION HACER CUANDO SE PRESIONA UN BOTON
volatile uint8_t bandera_estado = 0;						// VARIABLE DE BANDERA PARA INDICAR SI SE DEBE DE CAMBIA EL ESTADO EN ESTADO MANUAL
// VARIABLES PARA ADC
volatile uint16_t contador_ADC = 0;							// CONTADOR PARA CAMBIAR QUE CANAL DE ADC SE USA

// VARIABLES PARA MOTORES
volatile uint8_t ojo_izquierdo = 0;						// VARIABLES PARA GUARDAR EL VALOR DE ADC PARA MOVER CADA MOTOR
volatile uint8_t ojo_derecho = 0;
volatile uint8_t brazo_izquierdo = 0;
volatile uint8_t brazo_derecho = 0;
volatile uint8_t rueda_izquierda = 0;
volatile uint8_t rueda_derecha = 0;

// VARIABLES DE UART
volatile uint8_t receptor = 0;								// RECEPTOR DE MENSAJE
volatile uint8_t band_dato = 0;								// BANDERA PARA INDICAR QUE TODOS LOS DATOS SE HAYAN MANDADO
volatile uint8_t tipo_feed = 0;								// VARIABLE DONDE SE GUARDA DE QUE FEED VINO EL DATO
volatile uint8_t dato[2];									// ARRAY PARA GUARDAR DATOS QUE VIENEN DEL UART
volatile uint8_t contador_dato = 0;							// CONTADOR DE DATOS QUE SE ENVIAN DESDE UART

// VARIABLES DE EEPROM
const uint8_t pos_EEPROM[4] = {0x00, 0x06, 0x0C, 0x12};		// ARRAY PARA LAS POSICIONES INICIALES DONDE GUARDAR DATOS EN EEPROM
volatile uint8_t pos_write = 0;								// VARIABLE PARA DECIR DONDE ESCRIBE DENTRO DE LA EEPROM
volatile uint8_t pos_read = 0;								// VARIABLE PARA DECIRLE DONDE DEBE DE LEER EN LA EEPROM
	
/****************************************/
// Main Function
int main(void)
{
	setup();
	set_estado();
	while (1)
	{
		if (band_dato)
		{
			band_dato = 0;
			if (tipo_feed == 'E')
			{
				ESTADO_SEGUN_UART(receptor);
			} 
			else if ((tipo_feed == 'P') && (estado_actual == 1))
			{
				LEER_EEPROM(receptor);
			}
			else if ((tipo_feed == 'U') && (estado_actual == 2))
			{
				SETEAR_MOTOR(1, receptor);
			}
			else if ((tipo_feed == 'V') && (estado_actual == 2))
			{
				SETEAR_MOTOR(2, receptor);
			}
			else if ((tipo_feed == 'W') && (estado_actual == 2))
			{
				SETEAR_MOTOR(3, receptor);
			}
			else if ((tipo_feed == 'X') && (estado_actual == 2))
			{
				SETEAR_MOTOR(4, receptor);
			}
			else if ((tipo_feed == 'Y') && (estado_actual == 2))
			{
				SETEAR_MOTOR(5, receptor);
			}
			else if ((tipo_feed == 'Z') && (estado_actual == 2))
			{
				SETEAR_MOTOR(6, receptor);
			}
		}
		else if (estado_actual == 0)
		{
			MANUAL();
			if (bandera_guardar == 1)
			{
				ESCRIBIR_EEPROM();
				bandera_guardar = 0;
				writeString("G\n");
			}
		}
		else if (bandera_estado == 1)
		{
			bandera_estado = 0;
			set_estado();
		}
	}
}
/****************************************/
// NON-Interrupt subroutines
void setup()
{
	// DESHABILITAR INTERRUPCIONES GLOBALES
	cli();
	// HABILITAMOS PINES PARA BOTONES PARA CAMBIAR DE ESTADO CON PULLUPS
	DDRD	&=	~((1 << DDD4) | (1 << DDD2));
	PORTD   |=	(1 << PORTD4) | (1 << PORTD2);
	// HABILITAR INTERRUPCIONES DE PORTD
	PCICR	|= (1<<PCIE2);	// HABILITO INTS PARA PORTD
	PCMSK2	|= (1<<PCINT18) | (1<<PCINT20); // HABILITO INTS PARA EL PD2 Y EL PD4
	// LLAMAR INICIALIZAIONES
	PWM0_init();
	PWM1_init();
	PWM2_init();
	init_UART();
	ADC_init();
	// HABILITAR INTERRUPCIONES GLOBALES
	sei();
}
void ADC_init()
{
	// HABILITAR ADC CON VOLTAJE EXTERNO Y EN EL ADC0
	ADMUX = 0;
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	// HABILITAR ADC, SU INTERRUPCION Y PONER PRESCALER DE 128
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	ADCSRA |= (1 << ADSC);
}
void MANUAL()
{
	UPDATE_Duty_Cycle_PWM0A(6.0 + ((float)brazo_derecho * 31.0) / 255.0);
	UPDATE_Duty_Cycle_PWM0B(6.0 + ((float)brazo_izquierdo * 31.0) / 255.0);
	UPDATE_Duty_Cycle_PWM1A(800.0 + ((float)ojo_derecho * 4000.0) / 255.0);
	UPDATE_Duty_Cycle_PWM1B(800.0 + ((float)ojo_izquierdo * 4000.0) / 255.0);
	UPDATE_Duty_Cycle_PWM2A(6.0 + ((float)rueda_derecha * 31.0) / 255.0);
	UPDATE_Duty_Cycle_PWM2B(6.0 + ((float)rueda_izquierda * 31.0) / 255.0);
}
void set_estado()
{
	switch(estado_actual)
	{
	case 0:
		writeString("M\n");
		break;
	case 1:
		writeString("E\n");
		break;
	case 2:
		writeString("S\n");
		break;
	}
}
void ESTADO_SEGUN_UART(unsigned char estado)
{
	switch (estado)
	{
	case 'M':
		estado_actual = 0;
		break;
	case 'E':
		estado_actual = 1;
		break;
	case 'S':
		estado_actual = 2;
		break;
	default:
		estado_actual = 0;
		break;
	}
}
void ESCRIBIR_EEPROM()
{
	uint8_t direccion = pos_EEPROM[pos_write];
	for (uint8_t i = 0; i<6; i++)
	{
		switch (i)
		{
		case 0:
			eepromWrite(direccion,brazo_derecho);
			break;
		case 1:
			eepromWrite(direccion,brazo_izquierdo);
			break;
		case 2:
			eepromWrite(direccion,ojo_derecho);
			break;
		case 3:
			eepromWrite(direccion,ojo_izquierdo);
			break;
		case 4:
			eepromWrite(direccion,rueda_derecha);
			break;
		case 5:
			eepromWrite(direccion,rueda_izquierda);
			break;
		}
		direccion++;
	}
	pos_write = (pos_write + 1) & 0x03;
}
void LEER_EEPROM(uint8_t posicion)
{
	uint8_t direccion = pos_EEPROM[posicion];
	for (uint8_t i = 0; i<6; i++)
	{
		uint8_t dato_eeprom = eepromRead(direccion);
		switch (i)
		{
		case 0:
			UPDATE_Duty_Cycle_PWM0A(6.0 + ((float)dato_eeprom * 31.0) / 255.0);
			break;
		case 1:
			UPDATE_Duty_Cycle_PWM0B(6.0 + ((float)dato_eeprom * 31.0) / 255.0);
			break;
		case 2:
			UPDATE_Duty_Cycle_PWM1A(800.0 + ((float)dato_eeprom * 4000.0) / 255.0);
			break;
		case 3:
			UPDATE_Duty_Cycle_PWM1B(800.0 + ((float)dato_eeprom * 4000.0) / 255.0);
			break;
		case 4:
			UPDATE_Duty_Cycle_PWM2A(6.0 + ((float)dato_eeprom * 31.0) / 255.0);
			break;
		case 5:
			UPDATE_Duty_Cycle_PWM2B(6.0 + ((float)dato_eeprom * 31.0) / 255.0);
			break;
		}
		direccion++;
	}
}
void SETEAR_MOTOR(uint8_t n_motor, uint8_t valor)
{
	switch (n_motor)
	{
	case 1:
		brazo_derecho = valor;
		break;
	case 2:
		brazo_izquierdo = valor;
		break;
	case 3:
		ojo_derecho = valor;
		break;
	case 4:
		ojo_izquierdo = valor;
		break;
	case 5:
		rueda_derecha = valor;
		break;
	case 6:
		rueda_izquierda = valor;
		break;
	}
	MANUAL();
}
/****************************************/
// Interrupt routines
ISR(ADC_vect)
{
	switch (contador_ADC)
	{
		case 0:
			brazo_derecho = ADCH;
			ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX0);
			break;
		case 1:
			brazo_izquierdo = ADCH;
			ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX1);
			break;
		case 2:
			ojo_derecho	= ADCH;
			ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX1) | (1 << MUX0);
			break;
		case 3:
			ojo_izquierdo = ADCH;
			ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX2);
			break;
		case 4:
			rueda_derecha = ADCH;
			ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX2) | (1 << MUX0);
			break;
		case 5:
			rueda_izquierda	= ADCH;
			ADMUX = (1 << REFS0) | (1 << ADLAR);
			break;
		default:
		contador_ADC = 0x00;
		ADMUX = (1 << REFS0);
		break;
	}
	contador_ADC++;
	contador_ADC %= 6;
	ADCSRA |= (1 << ADSC);
}
ISR(PCINT2_vect)
{
	bandera_guardar = 0;
	if (!(PIND & (1 << PIND4)))
	{
		estado_actual++;
		estado_actual = estado_actual % 3;
		bandera_estado = 1;
	} 
	else if ((!(PIND & (1 << PIND2))) && (estado_actual == 0))
	{
		bandera_guardar = 1;
	}
}
ISR(USART_RX_vect)
{
	uint8_t valor = UDR0;
	dato[contador_dato++] = valor;
	if (contador_dato >= 2)
	{
		tipo_feed = dato[0];
		receptor = dato[1];
		
		contador_dato = 0;
		band_dato = 1;
	}
}