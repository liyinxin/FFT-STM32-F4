#include "adc.h"
#include "arm_math.h"  
#include "pwm.h"
#include "gpio.h"
#include "delay.h"


arm_cfft_radix4_instance_f32 scfft;

float fft_inputbuf1[FFT_LENGTH*2];	//FFT¨º?¨¨?¨ºy¡Á¨¦
float fft_outputbuf1[FFT_LENGTH];	//FFT¨º?3?¨ºy¡Á¨¦

float fft_inputbuf2[FFT_LENGTH*2];	//FFT¨º?¨¨?¨ºy¡Á¨¦
float fft_outputbuf2[FFT_LENGTH];	//FFT¨º?3?¨ºy¡Á¨¦

#define ADC1_DR_Addr    ((uint32_t)0x4001204C)	//ADC1µÄÊý¾Ý¼Ä´æÆ÷µØÖ·
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
Description: ADC DMA ¿ªÆô          
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
	ADC_SoftwareStartConv(ADC3);//¿ªÆôADC×ª»»
}

/*************************************************
Function:    void GPIO_Config(void)       
Description: ADCÏà¹ØGPIOÉèÖÃ          
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ADCÊ±ÖÓÊ¹ÄÜ
	ADC_DeInit();
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //      
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;   //É¨ÃèÄ£Ê½£¬É¨ÃèÄ£Ê½ÓÃÓÚ¶àÍ¨µÀ²É¼¯
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  //111//¿ªÆôÁ¬Ðø×ª»»Ä£Ê½£¬¼´²»Í£µØ½øÐÐADC×ª»»
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //²»Ê¹ÓÃÍâ²¿´¥·¢×ª»»
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //²É¼¯Êý¾ÝÓÒ¶ÔÆë
	ADC_InitStructure.ADC_NbrOfConversion = 2;    //Òª×ª»»µÄÍ¨µÀÊýÄ¿1
	ADC_Init(ADC1, &ADC_InitStructure);      

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;    //¶ÀÁ¢Ä£Ê½
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; //Ê±ÖÓ·ÖÆµ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA_MODE
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//²ÉÑùÊ±¼ä
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_15Cycles); //
	//ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE); //ADCÖÐ¶Ï
	ADC_Cmd(ADC1, ENABLE);  //ADC1Ê¹ÄÜ

	ADC_DMACmd(ADC1, ENABLE);   //ADC3 DMA 
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); //Ô´Êý¾Ý±ä»¯Ê±¿ªÆôDMA´«Êä


	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;//´¿Ú1ÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷¡¢
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//Ê±ÖÓ³õÊ¼»¯

	DMA_DeInit(DMA2_Stream0);//DMA2Êý¾ÝÁ÷0
	DMA_StructInit( &DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;           //
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Addr; //ADCÍâÉèµØÖ·
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCoverVaule;  //×Ô¶¨ÒåÄÚ´æÎ»ÖÃ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;    //??????????????

	DMA_InitStructure.DMA_BufferSize = (FFT_LENGTH*2);      //»º³åÇø´óÐ¡
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //ÍâÉèµØÖ·¹Ì¶¨
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    //ÄÚ´æµØÖ·²»¹Ì¶¨

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //°ë×Ö

	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //°ë×Ö
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;       //DMAÑ­»·´«Êä
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //ÓÅÏÈ¼¶High

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);      //Ê¹ÄÜDMA2_Stream0
	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);//¿ªÆôÏà¹ØÖÐ¶Ï

	//DMA NVIC ÅäÖÃ
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;//´®¿Ú1ÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷¡¢
}


void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///Ê¹ÄÜTIM3Ê±ÖÓ
	
       TIM_TimeBaseInitStructure.TIM_Period = arr; 	//×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//³õÊ¼»¯TIM3

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //
      TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
      TIM_OCInitStructure.TIM_Pulse = 1;                                                //!!??<=TIM_Period!!
      TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//

      TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //ÔÊÐí¶¨Ê±Æ÷3¸üÐÂÖÐ¶Ï
	TIM_Cmd(TIM2,ENABLE); //Ê¹ÄÜ¶¨Ê±Æ÷3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //¶¨Ê±Æ÷3ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //ÇÀÕ¼ÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///¨º1?¨¹TIM3¨º¡À?¨®
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//¡Á??¡¥??¡Á¡ã???¦Ì
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //?¡§¨º¡À?¡Â¡¤??¦Ì
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //?¨°¨¦???¨ºy?¡ê¨º?
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//3?¨º??¡¥TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //?¨ºD¨ª?¡§¨º¡À?¡Â3?¨¹D??D??
	TIM_Cmd(TIM3,ENABLE); //w¨º1?¨¹?¡§¨º¡À?¡Â3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //?¡§¨º¡À?¡Â3?D??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //?¨¤??¨®??¨¨??1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //¡Á¨®¨®??¨¨??3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//?¡§¨º¡À?¡Â3?D??¡¤t??o¡¥¨ºy

void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///¨º1?¨¹TIM3¨º¡À?¨®
	
  	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//¡Á??¡¥??¡Á¡ã???¦Ì
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //?¡§¨º¡À?¡Â¡¤??¦Ì
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //?¨°¨¦???¨ºy?¡ê¨º?
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//3?¨º??¡¥TIM3
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //?¨ºD¨ª?¡§¨º¡À?¡Â3?¨¹D??D??
	TIM_Cmd(TIM4,ENABLE); //¨º1?¨¹?¡§¨º¡À?¡Â3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //?¡§¨º¡À?¡Â3?D??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //?¨¤??¨®??¨¨??1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //¡Á¨®¨®??¨¨??3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void DMA2_Stream0_IRQHandler(void)
{
	u16 i;
    if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
	 {
		 

//		DMA_Cmd(DMA2_Stream0, DISABLE);      //Ê¹ÄÜDMA2_Stream0		
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
	TIM_Cmd(TIM2,DISABLE); //Ê¹ÄÜ¶¨Ê±Æ÷3
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
	TIM_Cmd(TIM3,DISABLE); //Ê¹ÄÜ¶¨Ê±Æ÷3
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
	TIM_Cmd(TIM4,DISABLE); //¨º1?¨¹?¡§¨º¡À?¡Â3
	TIM4->CNT=0;  //counter reset to 0
	return;
	}


 void ChaSamPeriod(u16 sampPeriod) //the unit is 1us.
 	{
	if(sampPeriod==0) return;
	TIM2_Int_Init(sampPeriod, 84-1);	//?¡§¨º¡À?¡Â¨º¡À?¨®84M¡ê?¡¤??¦Ì?¦Ì¨ºy8400¡ê??¨´¨°?84M/8400=10Khz¦Ì???¨ºy?¦Ì?¨º¡ê???¨ºy100¡ä??a10ms   
	return;
 	}

 

//¹¤ÒÕ¶Î²Ù×÷¶¨Ê±Æ÷ÖÐ¶Ï
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //Òç³öÖÐ¶Ï
		{

		}	
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //Çå³ýÖÐ¶Ï±êÖ¾Î»
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
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //¨°?3??D??
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
		
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //??3y?D??¡À¨º????
		}
}


void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //¨°?3??D??
		{
//			TIM_Cmd(TIM4,DISABLE); //1?¡À??¡§¨º¡À?¡Â3
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
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //??3y?D??¡À¨º????
}



void ADC_IRQHandler(void)   
{	
	 
	
	if (ADC_GetITStatus(ADC1, ADC_IT_EOC ) != RESET)
	 {
	  ADC_ClearITPendingBit(ADC1,ADC_IT_EOC );	  
	 }
}

