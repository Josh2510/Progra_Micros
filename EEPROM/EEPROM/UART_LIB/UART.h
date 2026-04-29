/*
 * UART.h
 *
 * Created: 4/22/2026 4:13:29 PM
 *  Author: Josue
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

void	init_UART();
void	writeChar(char caracter);
void	writeString(char* string);

#endif /* UART_H_ */