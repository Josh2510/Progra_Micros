/*
 * PWM0.h
 *
 * Created: 4/8/2026 5:16:46 PM
 *  Author: Josue
 */ 


#ifndef PWM0_H_
#define PWM0_H_

#include <avr/io.h>

#define no_invertido	0
#define invertido		1
#define fastPWM			0
#define phasePWM		1

void initPWM0A(uint8_t invert, uint8_t modo, uint16_t prescaler);
void initPWM0B(uint8_t invert, uint8_t modo, uint16_t prescaler);
void UPDATE_duty_cycle0A (uint8_t duty);
void UPDATE_duty_cycle0B (uint8_t duty);

#endif /* PWM0_H_ */