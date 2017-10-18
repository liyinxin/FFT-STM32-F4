#ifndef __ADC_H
#define	__ADC_H
#include "sys.h"
#include "stdio.h"	
#include "stm32f4xx.h"


#define FFT_LENGTH		256 		//FFT3∟?豕㏒???豕?那?1024米?FFT
extern float Mag_Max;
extern int Fn_Num;

extern uint16_t ADCoverVaule[FFT_LENGTH][2];
extern float fft_outputbuf2[FFT_LENGTH];	//FFT那?3?那y℅谷
extern float fft_outputbuf1[FFT_LENGTH];	//FFT那?3?那y℅谷
void Compare(void);
void GPIO_Config(void);
void ADC_Config(void);
void DMA_Config(void);
void ADC_DMA(void);

void TIM2_Int_Init(u16 arr,u16 psc);
void Timer2Stop(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
void Timer4FireSet(u16 arr);
void Timer4Stop(void);
void  ChaSamPeriod(u16 samplPeriod); //the unit is ms.
void startSampling(void);

void Timer4PulseSet(u16 arr, u16 pulseNum);// unit <<1us
#endif /* __ADC_H */

