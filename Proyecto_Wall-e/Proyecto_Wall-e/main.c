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
	// LLAMA AL SETUP DEL PROGRAMA
	setup();
	// LLAMA AL SETEO DEL MODO INICIAL QUE ES MANUAL
	set_estado();
	while (1)
	{
		if (band_dato) // SI LA BANDERA DE DATO SE ENCIENDE, ES QUE TODOS LOS DATOS ENVIADOS DESDE UART HAN LLEGADO
		{
			band_dato = 0; // SE APAGA LA BANDERA
			if (tipo_feed == 'E') // SE REVISA QUE TIPO DE FEED ES, E ES FEED ESTADO, P ES FEED DE POSICION DE EEPROM, Y DE U A Z SON MOTORES
			{
				ESTADO_SEGUN_UART(receptor);
			} 
			else if ((tipo_feed == 'P') && (estado_actual == 1)) // COMPRUEBA QUE TIPO DE FEEED ES Y SI ESTA EN EL ESTADO CORRECTO
			{													// ESTADO 0 ES MANUAL
				LEER_EEPROM(receptor);							// ESTADO 1 ES EEPROM
			}													// ESTADO 2 ES SERIAL
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
		else if (estado_actual == 0) // SI LA BANDERA DE DATOS NO ESTA ACTIVADA SE REVISA SI ES ESTADO MANUAL
		{
			MANUAL();	// SI ES ASI, SE LLAMA FUNCION DE ACTUALIZACION DE DATOS DE PWM
			if (bandera_guardar == 1)
			{ // SI LA BANDERA DE GUARDAR ESTA ENCENDIDA ES PORQUE SE QUIERE GUARDAR DATOS EN EEPROM
				ESCRIBIR_EEPROM();   // SE LLAMA FUNCION DE GUARDADO EN EEPROM
				bandera_guardar = 0; // SE APAGA BANDERA
				writeString("G\n");	// SE ENVIA UN STRING PARA QUE EN PYTHON SE IMPRIMA QUE SE PUDO GUARDAR EN EEPROM
			}
		}
		else if (bandera_estado == 1) // ESTA BANDERA FUNCIONA PARA VER SI SE DEBE DE CAMBIAR DE ESTADO
		{
			bandera_estado = 0; // SE APAGA LA BANDERA Y SE LLAMA A LA FUNCION DE CAMBIAR ESTADO, YA QUE SE HABRA AUMENTADO EL VALOR DE ESTADO EN LA INTERRUPCION
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
	// LLAMADA A CADA FUNCION PARA ACTUALIZAR LOS DATOS
	UPDATE_Duty_Cycle_PWM0A(6.0 + ((float)brazo_derecho * 31.0) / 255.0);
	UPDATE_Duty_Cycle_PWM0B(6.0 + ((float)brazo_izquierdo * 31.0) / 255.0);
	UPDATE_Duty_Cycle_PWM1A(800.0 + ((float)ojo_derecho * 2000.0) / 255.0);
	UPDATE_Duty_Cycle_PWM1B(800.0 + ((float)ojo_izquierdo * 2000.0) / 255.0);
	UPDATE_Duty_Cycle_PWM2A(6.0 + ((float)rueda_derecha * 31.0) / 255.0);
	UPDATE_Duty_Cycle_PWM2B(6.0 + ((float)rueda_izquierda * 31.0) / 255.0);
}
void set_estado()
{
	// SETEA LOS ESTADOS CON EL BOTON DEL CONTROL
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
	// SETEA ESTADOS DESDE EL ADAFRUIT
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
	// MANDA A ESCRIBIR EN EEPROM EN LA POSICION QUE ESTA
	uint8_t direccion = pos_EEPROM[pos_write];
	for (uint8_t i = 0; i<6; i++)
	{
		// SE HACE UN FOR DESDE 0 A 5 PARA GUARDAR TODOS LOS DATOS
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
		// SE AUMENTA LA DIRECCION CADA VEZ QUE SE TERMINA EL SWITCH
		direccion++;
	}
	// SE CICLA LA POSICION DE ESCRITURA ENTRE 0 Y 3
	pos_write = (pos_write + 1) & 0x03;
}
void LEER_EEPROM(uint8_t posicion)
{
	// PARA LEER DESDE EEPROM SE USA UNA POSICION QUE ES MANDADA DESDE DONDE SE LLAMA LA FUNCION
	uint8_t direccion = pos_EEPROM[posicion];
	for (uint8_t i = 0; i<6; i++)
	{
		// SE USA FOR PARA RECIBIR TODOS LOS DATOS Y DEPENDIENDO DE EN QUE NUMERO ESTE I, SE LLAMA UNA FUNCION DIFERENTE PARA ACTUALIZAR DATOS DE LOS PWM
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
		// SE AUMENTA LA DIRECCION DESPUES DEL SWITCH CASE
		direccion++;
	}
}
void SETEAR_MOTOR(uint8_t n_motor, uint8_t valor)
{
	// FUNCION QUE FUNCIONA CON COMUNICACION SERIAL, SE LE ENVIA EL NUMERO DE MOTOR Y QUE VALOR DEBE DE TENER Y USA UN SWITCH CASE PARA PONER ESE
	// VALOR EN LA VARIABLE CORRESPONDIENTE
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
	// DESPUES SE LLAMA A LA FUNCION MANUAL PARA QUE SE ACTUALICEN LOS DATOS
	MANUAL();
}
/****************************************/
// Interrupt routines
ISR(ADC_vect)
{
	// SE HACE UN SWITCH CASE PARA IR CAMBIANDO EL CANAL QUE SE LEE, DESPUES DE HABER HECHO UNA LECTURA
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
	contador_ADC++;						// DESPUES DEL SWITCH CASE SE AUMENTA EL CONTADOR
	contador_ADC %= 6;					// SE HACE EL RESIDUO DE UNA DIVISION CON 6 PARA QUE VAYA ROTANDO 0-1-2-3-4-5-0
	ADCSRA |= (1 << ADSC);				// SE VUELVE A LEER EL ADC
}
ISR(PCINT2_vect)
{
	bandera_guardar = 0;				// SE APAGA LA BANDERA DE GUARDAR
	if (!(PIND & (1 << PIND4)))			// SE REVISA SI ES EL PIND4
	{
		estado_actual++;				// SI ASI ES, SE AUMENTA LA VARIABLE DE ESTADO
		estado_actual = estado_actual % 3;	// SE CICLA ENTRE 0-1-2
		bandera_estado = 1;				// SE ENCIENDE BANDERA DE ESTADO
	} 
	else if ((!(PIND & (1 << PIND2))) && (estado_actual == 0))
	{
		bandera_guardar = 1;			// SI NO ES PIND4, REVISA SI ES PIND2, SI ASI ES Y ESTA EN ESTADO MANUAL, SE ACTIVA LA BANDERA DE GUARDAR.
	}
}
ISR(USART_RX_vect)
{
	uint8_t valor = UDR0;				// SE RECIBE DE LA COMUNICACION SERIAL UN VALOR
	dato[contador_dato++] = valor;		// SE METE EN UN ARRAY DE 2
	if (contador_dato >= 2)				// SE REVISA SI EL CONTADOR DE DATOS ES IGUAL A 2, SI ASI ES, ESO INDICA QUE SE MANDARON 2 BYTES DE INFORMACION
	{									// POR LO QUE SE ENTIENDE QUE SE ENVIO TODOS LOS DATOS QUE SE NECESITABAN
		tipo_feed = dato[0];			// LA PRIMERA POSICION SE PONE EN TIPO DE FEED
		receptor = dato[1];				// LA SEGUNDA POSICION SE PONE EN RECEPTOR, QUE ES UN VALOR O UNA LETRA
		
		contador_dato = 0;				// SE REINICIA LA VARIABLE DE CONTADOR DE DATOS
		band_dato = 1;					// SE ENCIENDE LA BANDERA DE DATOS
	}
}