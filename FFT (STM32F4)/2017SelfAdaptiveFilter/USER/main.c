#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "adc.h"
#include "key.h"  
#include "math.h" 
#include "gpio.h"
   
int main(void)
{ 
 	u16 adcx,i;
	u8 t=0;
	float temp;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	LED_Init();					//初始化LED 
// 	LCD_Init();         //初始化LCD接口
	uart_init(115200);	//串口初始化波特率为115200
	KEY_Init(); 				//按键初始化 
 	TIM2_Int_Init(100-1,84-1);	//10K   采样频率
	Timer2Stop();
	TIM3_Int_Init(10-1,8400-1);	//1ms    
	TIM4_Int_Init(500-1,8400-1);	//50ms    
	gpioInitiation();

	
  for(i=0;i<FFT_LENGTH;i++)
	{
	   ADCoverVaule[i][0]=0; 
	   ADCoverVaule[i][1]=0;
	}
	
	ADC_DMA();
  startSampling();
	while (1)
  {
		
		
		

	
  }
}



