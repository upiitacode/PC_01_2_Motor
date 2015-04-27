/**
 * @file main.c
 * This program generates a PWM signal on pin B3 with a ramp like duty cycle 
 * glowing a led attached to that pin (no included in board)
 */

#include "stm32f30x.h"                  // Device header
#include "timer2_ch2_pwm.h"
#include "serial_stdio.h"
#include "retarget_stm32f3.h"
#include <string.h>
/*Led PB13, Button PC13*/

void delay_ms(int delay_time);

void DIR_CTRL_Init(void);

Serial_t USART1_serial = {USART1_getChar, USART1_sendChar};

char myInBuffer[80];
char myOutBuffer[80];
char wordBuffer[80];

float current_speed=0;
float command_speed=0;

int main(){
	int newSpeed,tokensFound;
	float error;
	//Values calculates for 1000KHz signal
	short myPrescaler=0xF;
	int myAutorreload=5000;
	//Initialize PWM
	TIMER2_CH2_PWM_Init(myPrescaler,myAutorreload);
	TIMER2_CH2_PWM_SetDutyCycle(0,myAutorreload);
	//Initialize Direction control signals
	DIR_CTRL_Init();
	//Initialize USART
	USART1_init(9600);
	serial_puts(USART1_serial,"\nSystem ready\n");
	
	while(1){
		serial_gets(USART1_serial,myInBuffer,80);
		tokensFound = sscanf(myInBuffer,"V%d",&newSpeed);
		if(tokensFound == 1){
			serial_printf(USART1_serial,"new Speed = %d\n",newSpeed);
			command_speed=newSpeed;
		}
		//Glow the led every 3 seconds aprox
		while(1){
			error=command_speed-current_speed;			
			if(error > 0.25){
				current_speed+=0.5;
			}else if(error < -0.25){
				current_speed-=0.5f;
			}else{
				break;
			}
			//direction control
			if(current_speed<0){
				GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET);
				GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET);
			}else{
				GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET);
				GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET);
			}
			TIMER2_CH2_PWM_SetDutyCycle(((current_speed>0) ? current_speed : (-current_speed)),myAutorreload);
			delay_ms(0x1900);
		}
	}
}

/*PB4 and PB5 Control signals*/
void DIR_CTRL_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	/*Enable peripheral clock for GPIOB*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);	
	/*GPIOA Configuration PA4 and PB5*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, (GPIO_Pin_4 | GPIO_Pin_5));
}

void delay_ms(int delay_time){
	for(int i=0; i<delay_time; i++);
}
