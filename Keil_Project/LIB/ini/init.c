#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "init.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"



void TIM4_ini(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_TimeBaseInitTypeDef timer_init;
	TIM_TimeBaseStructInit(&timer_init);
	timer_init.TIM_Period = PWM_period-1;// ---> 20kHz
	timer_init.TIM_Prescaler = 2-1; // ---> 42 MHz
	TIM_TimeBaseInit(TIM4, &timer_init);
	
	//channel 1;	
	TIM_OCInitTypeDef tim_oc_init1;
	TIM_OCStructInit(&tim_oc_init1);
	tim_oc_init1.TIM_Pulse = 0;  
	tim_oc_init1.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_init1.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(TIM4, &tim_oc_init1);// 
	
	
	//channel 2;
	TIM_OCInitTypeDef tim_oc_init2;
	TIM_OCStructInit(&tim_oc_init2);
	tim_oc_init2.TIM_Pulse = 0;  
	tim_oc_init2.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_init2.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC2Init(TIM4, &tim_oc_init2);// 
	
	//channel 3;
	TIM_OCInitTypeDef tim_oc_init3;
	TIM_OCStructInit(&tim_oc_init3);
	tim_oc_init3.TIM_Pulse = 0;  
	tim_oc_init3.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_init3.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM4, &tim_oc_init3);// 
	
	
	TIM_Cmd(TIM4, ENABLE);
	
	
	
}



// PWM out for 12th pin

void PWM_INx_init(void) // INx, IN - PWMpins
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	
	//IN1 --> PD12--------------------------
	GPIO_InitTypeDef init_AF;//
	GPIO_StructInit(&init_AF);// 
	init_AF.GPIO_Mode = GPIO_Mode_AF;//
	init_AF.GPIO_Pin = GPIO_Pin_12;// 
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);// 
	GPIO_Init(GPIOD, &init_AF);// 
	
	//IN2 --> PD13--------------------------
	
	GPIO_StructInit(&init_AF);// 
	init_AF.GPIO_Mode = GPIO_Mode_AF;//
	init_AF.GPIO_Pin = GPIO_Pin_13;// 
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);// 
	GPIO_Init(GPIOD, &init_AF);//
	
	
	//IN3 --> PD14--------------------------

	GPIO_StructInit(&init_AF);// 
	init_AF.GPIO_Mode = GPIO_Mode_AF;//
	init_AF.GPIO_Pin = GPIO_Pin_14;// 
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);// 
	GPIO_Init(GPIOD, &init_AF);//
		
}



void ENx_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef ENx_init;
	GPIO_StructInit(&ENx_init);
	ENx_init.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5  ;
	ENx_init.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA,&ENx_init);
	
		
}








void ADC_initt(void) // PB1
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	GPIO_InitTypeDef GPIO_AI;
	GPIO_StructInit(&GPIO_AI);
	GPIO_AI.GPIO_Pin = GPIO_Pin_1;
	GPIO_AI.GPIO_Mode = GPIO_Mode_AN;
	GPIO_AI.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_AI.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_AI);
	
	
	
	ADC_CommonInitTypeDef ADC_init;
	ADC_InitTypeDef ADC_InitStructure;

	ADC_StructInit(&ADC_InitStructure);
	ADC_CommonStructInit(&ADC_init);
	ADC_CommonInit (&ADC_init);
	ADC_Init(ADC1, &ADC_InitStructure);
	
	
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	
	
			NVIC_InitTypeDef NVIC_InitStructure;

NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
	
	
	
	ADC_Cmd(ADC1, ENABLE);
//NVIC_EnableIRQ(ADC_IRQn);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_480Cycles);
	ADC_SoftwareStartConv(ADC1);
	
}





 void USART_2_init(void) //PD6,PD5
 {
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	 
	GPIO_InitTypeDef USART2_ini;//
	USART2_ini.GPIO_Mode = GPIO_Mode_AF;//
	USART2_ini.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;// 
	USART2_ini.GPIO_Speed = GPIO_Speed_2MHz;
  USART2_ini.GPIO_OType = GPIO_OType_PP;
  USART2_ini.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOD, &USART2_ini);// 
	 
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);// 
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);//
	 
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE) ;
	 
	USART_InitTypeDef USART2_user;
	USART2_user.USART_BaudRate= 115200;
	USART2_user.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	 USART2_user.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	 USART2_user.USART_Parity = USART_Parity_No;
	 USART2_user.USART_StopBits = USART_StopBits_1;
	 USART2_user.USART_WordLength = USART_WordLength_8b;
	 
	 USART_Init(USART2, &USART2_user);
	 
	 
	 	NVIC_InitTypeDef NVIC_InitStructure2;

NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn;
NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure2);
	 
	 
	 
	 //NVIC_EnableIRQ(USART2_IRQn);
	 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	 
	 
	 
	 USART_Cmd(USART2, ENABLE);
 }

void user_button_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef User_But_ini;//
	User_But_ini.GPIO_Mode = GPIO_Mode_IN;//
	User_But_ini.GPIO_Pin = GPIO_Pin_0;// 
	User_But_ini.GPIO_Speed = GPIO_Speed_2MHz;
  User_But_ini.GPIO_OType = GPIO_OType_PP;
  User_But_ini.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOA, &User_But_ini);//
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	
	
	
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	
}










void TIM1_ini(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseInitTypeDef timer_init;
	TIM_TimeBaseStructInit(&timer_init);
	timer_init.TIM_Period = 8400-1;// ---> 100 microsec
	timer_init.TIM_Prescaler = 2-1; // ---> 42 MHz
	TIM_TimeBaseInit(TIM1, &timer_init);
	
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	
	
	TIM_Cmd(TIM1, ENABLE);
	
	
	NVIC_InitTypeDef NVIC_InitStructure1;

NVIC_InitStructure1.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure1);
	
 // NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn );
	 
	

	
	
	
}


void TIM2_ini(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitTypeDef timer2_init;
	TIM_TimeBaseStructInit(&timer2_init);
	timer2_init.TIM_Period = 42000-1;// ---> 1000 microsec
	timer2_init.TIM_Prescaler = 2-1; // ---> 42 MHz
	TIM_TimeBaseInit(TIM2, &timer2_init);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	
		NVIC_InitTypeDef NVIC_InitStructure;

NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
	
	
	
	TIM_Cmd(TIM2, ENABLE);
	
//  NVIC_EnableIRQ(TIM2_IRQn );
	 
	

	
	
	
}