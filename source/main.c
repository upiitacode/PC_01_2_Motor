/**
 * @file main.c
 * This program generates a PWM signal on pin B3 with a ramp like duty cycle 
 * glowing a led attached to that pin (no included in board)
 */

#include "stm32f30x.h"                  // Device header
#include "timer2_ch2_pwm.h"
#include <stdio.h>
#include <string.h>
/*Led PB13, Button PC13*/

void delay_ms(int delay_time);

void USART2_init(void);
void USART2_sendChar(char ch);
char USART2_getChar(void);

void s_puts(const char * pString);
int s_gets(char * pBuffer, int bufferSize);
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
	//Initialize USART
	USART2_init();
	s_puts("\nSystem ready\n");
	SystemCoreClockUpdate();
	while(1){
		s_gets(myInBuffer,80);
		tokensFound = sscanf(myInBuffer,"V%d",&newSpeed);
		if(tokensFound == 1){
			snprintf(myOutBuffer,80,"new Speed = %d\n",newSpeed);
			s_puts(myOutBuffer);
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
			TIMER2_CH2_PWM_SetDutyCycle(current_speed,myAutorreload);
			delay_ms(0x5FFF);
		}
	}
}

int s_gets(char * pBuffer, int bufferSize){
	char newChar; 
	int i ;
	for(i = 0 ; i < (bufferSize - 1); i++){
		newChar = USART2_getChar();
		if( (newChar != '\r') ){
			pBuffer[i]=newChar;
		}else{
			pBuffer[i]='\0';
			return i;
		}
	}
	pBuffer[bufferSize-1]='\0';
	return bufferSize-1;
}

void s_puts(const char * pString){
	char newChar; 
	int i ;
	for(i = 0 ; i < 81; i++){
		newChar = pString[i];
		if( newChar != '\0' ){
			USART2_sendChar(newChar);
		}else{
			break;
		}
		
	}
}

void USART2_sendChar(char ch){
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
	
	USART_SendData(USART2,ch);
}

char USART2_getChar(void){
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)) USART_ClearFlag(USART2,USART_FLAG_ORE); 
	
	while(!USART_GetFlagStatus(USART2,USART_FLAG_RXNE));
	return USART_ReceiveData(USART2);
}

void USART2_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/*Enable peripheral clock for GPIOA*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	/*Enable peripheral clock for UART2*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	/*GPIOA Configuration PA2 as TX PA3 as RX*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*Connect USART2 pins to AF2*/
	//TX=PA2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_7);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_7);
	
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_Init(USART2,&USART_InitStructure);
	USART_Cmd(USART2,ENABLE);
}

void delay_ms(int delay_time){
	for(int i=0; i<delay_time; i++);
}
