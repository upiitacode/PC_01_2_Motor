#include "timer2_ch2_pwm.h"

void TIMER2_CH2_PWM_Init(int prescaler,int autoreload){
	//USER LED / PB3  / TIM2_CH2 / AF1
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOBEN ,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Pin=GPIO_Pin_3;
	myGPIO.GPIO_Mode=GPIO_Mode_AF;
	myGPIO.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&myGPIO);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_1);
	//select the output mode by writing the CCS bits in the CCMRx register
	
	//Timer time base configuration
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
	TIM_TimeBaseInitTypeDef myTimeBase;
	TIM_TimeBaseStructInit(&myTimeBase);
	myTimeBase.TIM_CounterMode=TIM_CounterMode_Up;
	myTimeBase.TIM_Period=autoreload;
	myTimeBase.TIM_Prescaler=prescaler;
	myTimeBase.TIM_ClockDivision= TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&myTimeBase);
	//Timer capture compare configuration
	TIM_OCInitTypeDef myTimerOC;
	TIM_OCStructInit(&myTimerOC);
	myTimerOC.TIM_OCMode=TIM_OCMode_PWM1;
	myTimerOC.TIM_OCPolarity=TIM_OCPolarity_Low;
	myTimerOC.TIM_OutputState=TIM_OutputState_Enable;
	myTimerOC.TIM_Pulse=autoreload;//0 Duty cycle at start
	TIM_OC2Init(TIM2,&myTimerOC);
	TIM_CCxCmd(TIM2,TIM_Channel_2,TIM_CCx_Enable);//enable CCP2
	//start Timer
	TIM_Cmd(TIM2,ENABLE);//Counter enabled
}

void TIMER2_CH2_PWM_SetDutyCycle(float dutyCycle, int autoreload){
	int newDC=0;// new duty cycle
	newDC=(int )((dutyCycle/100.0)*autoreload);
	TIM_SetCompare2(TIM2,newDC);
}
