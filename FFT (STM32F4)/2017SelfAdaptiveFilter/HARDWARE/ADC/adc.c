#include "adc.h"
#include "arm_math.h"  
#include "pwm.h"
#include "gpio.h"
#include "delay.h"


arm_cfft_radix4_instance_f32 scfft;

float fft_inputbuf1[FFT_LENGTH*2];	//FFTê?è?êy×é
float fft_outputbuf1[FFT_LENGTH];	//FFTê?3?êy×é

float fft_inputbuf2[FFT_LENGTH*2];	//FFTê?è?êy×é
float fft_outputbuf2[FFT_LENGTH];	//FFTê?3?êy×é

#define ADC1_DR_Addr    ((uint32_t)0x4001204C)	//ADC1的数据寄存器地址
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
Description: ADC DMA 开启          
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
	ADC_SoftwareStartConv(ADC3);//开启ADC转换
}

/*************************************************
Function:    void GPIO_Config(void)       
Description: ADC相关GPIO设置          
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ADC时钟使能
	ADC_DeInit();
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //      
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;   //扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  //111//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; //不使用外部触发转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //采集数据右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 2;    //要转换的通道数目1
	ADC_Init(ADC1, &ADC_InitStructure);      

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;    //独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; //时钟分频
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA_MODE
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//采样时间
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_15Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_15Cycles); //
	//ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE); //ADC中断
	ADC_Cmd(ADC1, ENABLE);  //ADC1使能

	ADC_DMACmd(ADC1, ENABLE);   //ADC3 DMA 
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); //源数据变化时开启DMA传输


	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;//纯�1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//时钟初始化

	DMA_DeInit(DMA2_Stream0);//DMA2数据流0
	DMA_StructInit( &DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;           //
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Addr; //ADC外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCoverVaule;  //自定义内存位置
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;    //??????????????

	DMA_InitStructure.DMA_BufferSize = (FFT_LENGTH*2);      //缓冲区大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    //内存地址不固定

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //半字

	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //半字
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;       //DMA循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //优先级High

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);      //使能DMA2_Stream0
	
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);//开启相关中断

	//DMA NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}


void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM3时钟
	
       TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM3

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                //
      TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
      TIM_OCInitStructure.TIM_Pulse = 1;                                                //!!??<=TIM_Period!!
      TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//

      TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ê1?üTIM3ê±?ó
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//×??ˉ??×°???μ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //?¨ê±?÷·??μ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //?òé???êy?￡ê?
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//3?ê??ˉTIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //?êDí?¨ê±?÷3?üD??D??
	TIM_Cmd(TIM3,ENABLE); //wê1?ü?¨ê±?÷3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //?¨ê±?÷3?D??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //?à??ó??è??1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //×óó??è??3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//?¨ê±?÷3?D??·t??oˉêy

void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ê1?üTIM3ê±?ó
	
  	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//×??ˉ??×°???μ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //?¨ê±?÷·??μ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //?òé???êy?￡ê?
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//3?ê??ˉTIM3
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //?êDí?¨ê±?÷3?üD??D??
	TIM_Cmd(TIM4,ENABLE); //ê1?ü?¨ê±?÷3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //?¨ê±?÷3?D??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //?à??ó??è??1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //×óó??è??3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void DMA2_Stream0_IRQHandler(void)
{
	u16 i;
    if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
	 {
		 

//		DMA_Cmd(DMA2_Stream0, DISABLE);      //使能DMA2_Stream0		
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
	TIM_Cmd(TIM2,DISABLE); //使能定时器3
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
	TIM_Cmd(TIM3,DISABLE); //使能定时器3
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
	TIM_Cmd(TIM4,DISABLE); //ê1?ü?¨ê±?÷3
	TIM4->CNT=0;  //counter reset to 0
	return;
	}


 void ChaSamPeriod(u16 sampPeriod) //the unit is 1us.
 	{
	if(sampPeriod==0) return;
	TIM2_Int_Init(sampPeriod, 84-1);	//?¨ê±?÷ê±?ó84M￡?·??μ?μêy8400￡??ùò?84M/8400=10Khzμ???êy?μ?ê￡???êy100′??a10ms   
	return;
 	}

 

//工艺段操作定时器中断
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
		{

		}	
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
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
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //ò?3??D??
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
		
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //??3y?D??±ê????
		}
}


void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //ò?3??D??
		{
//			TIM_Cmd(TIM4,DISABLE); //1?±??¨ê±?÷3
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
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //??3y?D??±ê????
}



void ADC_IRQHandler(void)   
{	
	 
	
	if (ADC_GetITStatus(ADC1, ADC_IT_EOC ) != RESET)
	 {
	  ADC_ClearITPendingBit(ADC1,ADC_IT_EOC );	  
	 }
}

