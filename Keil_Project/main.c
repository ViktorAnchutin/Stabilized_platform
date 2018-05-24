#include "stm32f4xx.h" 
#include "as5048.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "mylib.h"
#include "init.h"
#include "motor.h"
#include <stdio.h>


uint8_t mode;
float angle, angle_error, angle_test, angle1;
float des_val,des_val1;
uint32_t t1, t2, t3, t4, dt1, dt22, p, timex, t1_IMU, t2_IMU, dt_IMU;

float voltage, current;

float sin_x, cos_x, tv_g, t_g, t_d;

uint8_t ADC_started;

// moving average filter variables


uint8_t filled_ADC;
	uint16_t i_ADC; // counter for filling in
	float data_ADC;
	float arr_ADC[window_ADC],  angle_average, a_i_ADC,  des_val_raw; // variables for first order moving average
	uint16_t k_ADC; // counter
	float ADC_average;
	uint8_t flag_ADC;
  uint32_t sum_ADC;
	
	
	//moving average Roll
	
	uint8_t filled_Roll;
	uint32_t i_Roll; // counter for filling in
	float data_Roll;
	uint32_t arr_Roll[window_Roll],  Roll_average, a_i_Roll; // variables for first order moving average
	uint32_t k_Roll; // counter
	
	uint8_t flag_Roll;
  uint32_t sum_Roll;
	
	
	
	
	
	
	
	


uint16_t USART_count;
uint8_t IMU_data_ready;

uint16_t IMU_Recieve_Buf[11];
uint8_t IMU_count;
uint16_t USART_test_rec;
uint8_t IMU_data;

float Pitch, Roll, Yaw;
float Roll_raw, roll_sine, roll_cos, Roll_raw_test, roll_sine_test, roll_cos_test, Roll_test;

 moving_average_type ADC_filter;
 
 
 
 
 
 
 
 

int main(void)
	
{
	
	
	
	//USART_2_init();
	SPI3_ini();
	user_button_init();
	
	
	TIM5_ini(); // Delay timer
		
	TIM4_ini(); // PWM timer	
	
	PWM_INx_init();
	ENx_init();
	ADC_initt();
	Set_nRes_nSleep();
	Set_ENx();
	FOC_InitPosition();
	
	des_val = 36; ///////////////


	while(1)
	{
		
		
		if(!ADC_filter.filled)
		{
			while(!ADC_filter.filled); // wait for adc filter ready
		}
		
		
			
			t1 = TIM5->CNT;
		
	angle = CQ_average_angle();//ThirdOrder_average();//average_angle();//	angle = get_angle();
		
  des_val = ADC_average*360/4095;
  
	angle_error = des_val - angle;
	
	if(mode==0)	FOC(angle, angle_error, 1,   0,  0.01,  dt1)	;		
 
	if(mode==1) sinus_control_V2(angle_error);
		
	if(mode==2)combined_control_V3(angle, angle_error, 0.2 , 0, 0, 0);
		
			t2 = TIM5->CNT;
			dt1 = t2 - t1;
	

		
	
	}
}




void EXTI0_IRQHandler(void) {
	
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {		
			
			EXTI_ClearITPendingBit(EXTI_Line0);
		 NVIC_DisableIRQ(EXTI0_IRQn);
		
		mode++;
			if(mode>2) mode=0;
		
		NVIC_EnableIRQ(EXTI0_IRQn);
		
	}
	
}




void ADC_IRQHandler()
	{
		if(ADC_GetITStatus(ADC1,ADC_IT_EOC) !=RESET)
		{
			
			
			ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
			
			ADC_average =  moving_average(&ADC_filter, (float)ADC_GetConversionValue(ADC1), 2000);
						
			ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_480Cycles);
	    
			ADC_SoftwareStartConv(ADC1);
									
		}
		
	}  
	
	