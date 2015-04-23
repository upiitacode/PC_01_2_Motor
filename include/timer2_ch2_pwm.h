#ifndef _TIMER2_CH2_PWM_H
#define _TIMER2_CH2_PWM_H

#include "stm32f30x.h"                  // Device header

/**
 * Initialize the TIMER2 Comparator 2 as a PWM generator on pin B3
 * with frecuency = CLK_sys/((prescaler+1)*autorreload) and duty 
 * cycle = 0  
 * @param prescaler values from 0 to 6
 * @param autoreload 
 */
void TIMER2_CH2_PWM_Init(int prescaler,int autoreload);

/**
 * Set the duty cycle of TIMER2 CH2 on pin B3
 * @param dutyCycle values from 0.0 to 100.0  
 * @param atoreload the value used as autoreload when calling
 * TIMER2_CH2_PWM_Init
 */
void TIMER2_CH2_PWM_SetDutyCycle(float dutyCycle, int atoreload);

#endif
