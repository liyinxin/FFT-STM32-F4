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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	LED_Init();					//��ʼ��LED 
// 	LCD_Init();         //��ʼ��LCD�ӿ�
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	KEY_Init(); 				//������ʼ�� 
 	TIM2_Int_Init(100-1,84-1);	//10K   ����Ƶ��
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



