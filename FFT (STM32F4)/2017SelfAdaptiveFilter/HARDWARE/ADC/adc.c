#include "adc.h"
#include "arm_math.h"  
#include "pwm.h"
#include "gpio.h"
#include "delay.h"


arm_cfft_radix4_instance_f32 scfft;

float fft_inputbuf1[FFT_LENGTH*2];	//FFT��?��?��y����
float fft_outputbuf1[FFT_LENGTH];	//FFT��?3?��y����

float fft_inputbuf2[FFT_LENGTH*2];	//FFT��?��?��y����
float fft_outputbuf2[FFT_LENGTH];	//FFT��?3?��y����

#define ADC1_DR_Addr    ((uint32_t)0x4001204C)	//ADC1�����ݼĴ�����ַ
#define ADC3_DR_Addr   ((uint32_t)0x4001224C)
//u16 ADC_ConvertedValue[6];
uint16_t ADCoverVaule[FFT_LENGTH][2];

u8 FFTProcessFlag=0;
u16 PulseNumLeft;
float Mag_Max;
int Fn_Num;

void startSampling(void)
	{
	ChaSamPeriod(100);
	}

/*************************************************
Function:    void ADC_DMA(void)       
Description: ADC DMA ����          
Input:                         
Output:                              
Return:                            
*************************************************/ 

void ADC_DMA(void)
{
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);//3
	GPIO_Config();
	ADC_Config();
	DMA_Config();
	ADC_SoftwareStartConv(ADC3);//����ADCת��
}

/*************************************************
Function:    void GPIO_Config(void)       
Description: ADC���GPIO����          
Input:                         
Output:                              
Return:                            
*************************************************/ 
void GPIO_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
GPIO_StructInit(&GPIO_InitStructure);
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*************************************************
Function:    void ADC_Config(void)      
Description: ADC????             
Input: ?                                
Output:?                              
Return:?                              
*************************************************/
void ADC_Config(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;

	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ADCʱ��ʹ��
	ADC_DeInit();
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //      
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;   //ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  //111//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 2;    //Ҫת����ͨ����Ŀ1
	ADC_Init(ADC1, &ADC_InitStructure);      

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;    //����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; //ʱ�ӷ�Ƶ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA_MODE
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//����ʱ��
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_15Cycles); //
	//ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE); //ADC�ж�
	ADC_Cmd(ADC1, ENABLE);  //ADC1ʹ��

	ADC_DMACmd(ADC1, ENABLE);   //ADC3 DMA 
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); //Դ���ݱ仯ʱ����DMA����


	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;//���1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

/*************************************************
Function:    void DMA_Config(void)  
Description: DMA                
Input:                    
Output:                         
Return:                          
*************************************************/
void DMA_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//ʱ�ӳ�ʼ��

	DMA_DeInit(DMA2_Stream0);//DMA2������0
	DMA_StructInit( &DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;           //
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Addr; //ADC�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCoverVaule;  //�Զ����ڴ�λ��
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;    //??????????????

	DMA_InitStructure.DMA_BufferSize = (FFT_LENGTH*2);      //��������С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    //�ڴ��ַ���̶�

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //����

	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //����
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;       //DMAѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //���ȼ�High

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);      //ʹ��DMA2_Stream0
	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);//��������ж�

	//DMA NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}


void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM3ʱ��
	
       TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM3

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //
      TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
      TIM_OCInitStructure.TIM_Pulse = 1;                                                //!!??<=TIM_Period!!
      TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//

      TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///��1?��TIM3����?��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//��??��??����???��
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //?������?�¡�??��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //?����???��y?�꨺?
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//3?��??��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //?��D��?������?��3?��D??D??
	TIM_Cmd(TIM3,ENABLE); //w��1?��?������?��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //?������?��3?D??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //?��??��??��??1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //������??��??3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//?������?��3?D??��t??o����y

void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///��1?��TIM3����?��
	
  	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//��??��??����???��
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //?������?�¡�??��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //?����???��y?�꨺?
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//3?��??��TIM3
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //?��D��?������?��3?��D??D??
	TIM_Cmd(TIM4,ENABLE); //��1?��?������?��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //?������?��3?D??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //?��??��??��??1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //������??��??3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void DMA2_Stream0_IRQHandler(void)
{
	u16 i;
    if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
	 {
		 

//		DMA_Cmd(DMA2_Stream0, DISABLE);      //ʹ��DMA2_Stream0		
		Timer2Stop();
		
		for(i=0;i<FFT_LENGTH;i++)
		{
		fft_inputbuf1[2*i]=(float)ADCoverVaule[i][0];
		fft_inputbuf1[2*i+1]=0;
		fft_inputbuf2[2*i]=(float)ADCoverVaule[i][1];
		fft_inputbuf2[2*i+1]=0;
		
		}
		
	 	TestGPIO2=~TestGPIO2;
	 DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);	
	 FFTProcessFlag=1;
	}
}

void Timer2Stop()
	{
	TIM_Cmd(TIM2,DISABLE); //ʹ�ܶ�ʱ��3
	TIM2->CNT=0;  //counter reset to 0
	return;
	}


void Timer3FireSet(u32 arr)
	{
	TIM3->ARR=arr;  // time till fire.
	TIM3->PSC=8400-1;  // period  	 
	TIM3->CNT=0;  //counter reset to 0
	TIM3->CR1|=0x01;    //start counting.
	}

void Timer3Stop()
	{
	TIM_Cmd(TIM3,DISABLE); //ʹ�ܶ�ʱ��3
	TIM3->CNT=0;  //counter reset to 0
	return;
	}


void Timer4PulseSet(u16 arr, u16 pulseNum)// unit <<1us
	{
	PulseNumLeft=(pulseNum*2);
	TIM4->ARR=arr;  // time till fire.
//	TIM4->PSC=10-1;  // period  	 
	TIM4->PSC=84-1;  // period  	 
	TIM4->CNT=0;  //counter reset to 0
	TIM4->CR1|=0x01;    //start counting.
	}

void Timer4Stop()
	{
	TIM_Cmd(TIM4,DISABLE); //��1?��?������?��3
	TIM4->CNT=0;  //counter reset to 0
	return;
	}


 void ChaSamPeriod(u16 sampPeriod) //the unit is 1us.
 	{
	if(sampPeriod==0) return;
	TIM2_Int_Init(sampPeriod, 84-1);	//?������?�¨���?��84M��?��??��?�̨�y8400��??����?84M/8400=10Khz��???��y?��?����???��y100��??a10ms   
	return;
 	}

 

//���նβ�����ʱ���ж�
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
		{

		}	
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}
void Compare(void)
{
	int i;
	Mag_Max = fft_outputbuf1[1];
	for(i=1;i<FFT_LENGTH/2;i++)
	{
		if(Mag_Max < fft_outputbuf1[i])
		{
			Fn_Num = i;
			Mag_Max = fft_outputbuf1[i];
		}
	}
}

void TIM3_IRQHandler(void)
{
u16 i;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //��?3??D??
		{
		if(FFTProcessFlag==1)
			{
			arm_cfft_radix4_f32(&scfft,fft_inputbuf1);	
			arm_cmplx_mag_f32(fft_inputbuf1,fft_outputbuf1,FFT_LENGTH);	
			arm_cfft_radix4_f32(&scfft,fft_inputbuf2);	
			arm_cmplx_mag_f32(fft_inputbuf2,fft_outputbuf2,FFT_LENGTH);	
			
 Compare();
		printf("ping lv = %f ",(float)10000*Fn_Num/256);
		


		
		FFTProcessFlag=0;	
		ChaSamPeriod(100);
		}
		
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //??3y?D??����????
		}
}


void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //��?3??D??
		{
//			TIM_Cmd(TIM4,DISABLE); //1?��??������?��3
		if(PulseNumLeft>0)
			{
			PulseGPIO=~PulseGPIO;
		 
			PulseNumLeft--;
			if(PulseNumLeft==0)
				{
				TIM_Cmd(TIM4,DISABLE); //1
				}
			}

		}
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //??3y?D??����????
}



void ADC_IRQHandler(void)   
{	
	 
	
	if (ADC_GetITStatus(ADC1, ADC_IT_EOC ) != RESET)
	 {
	  ADC_ClearITPendingBit(ADC1,ADC_IT_EOC );	  
	 }
}

