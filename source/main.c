/**
 * @file main.c
 * This program generates a PWM signal on pin B3 with a ramp like duty cycle 
 * glowing a led attached to that pin (no included in board)
 */

#include "stm32f30x.h"                  // Device header
#include "timer2_ch2_pwm.h"
/*Led PB13, Button PC13*/

void delay_ms(int delay_time);

int main(){
	//Values calculates for 1000KHz signal
	short myPrescaler=0xF;
	int myAutorreload=5000;
	//Initialize PWM
	TIMER2_CH2_PWM_Init(myPrescaler,myAutorreload);
	while(1){
		//Glow the led every 3 seconds aprox
		for(int i=0; i<100; i++){
			TIMER2_CH2_PWM_SetDutyCycle(i,myAutorreload);
			delay_ms(0x3FFF);
		}
	}
}

void delay_ms(int delay_time){
	for(int i=0; i<delay_time; i++);
}
