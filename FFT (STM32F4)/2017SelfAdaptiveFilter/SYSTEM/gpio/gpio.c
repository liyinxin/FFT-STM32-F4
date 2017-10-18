#include "gpio.h"


void gpioAppInitiation(void)
	{

	  GPIO_InitTypeDef  GPIO_InitStructure;

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//

	//portA
	  GPIO_InitStructure.GPIO_Pin = Sampling_ChannelSel_Pin | SamplingAd630Sel_A_Pin | SamplingAd630Sel_B_Pin | DC_Direction_Ctrl_Pin;//
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
//        GPIO_SetBits(GPIOA,Sampling_ChannelSel_Pin | SamplingAd630Sel_A_Pin | SamplingAd630Sel_B_Pin | DC_Direction_Ctrl_Pin);//

	//Sampling_ChannelSel_Pin=0 ----> PD1 is on. Otherwise, Sampling_ChannelSel_Pin=1 ----> PD2 is on.
        GPIO_ResetBits(GPIOA, Sampling_ChannelSel_Pin);//

       //SamplingAd630Sel_A_Pin is initialized to "1".
        GPIO_SetBits(GPIOA, SamplingAd630Sel_A_Pin);//

	//SamplingAd630Sel_B_Pin does not matter. It is normally unconnected.
        GPIO_ResetBits(GPIOA, SamplingAd630Sel_B_Pin);//

	//DC_Direction_Ctrl_Pin=0 ----> motor forward totation DC_Direction_Ctrl_Pin=1 ----> motor backforward totation
        GPIO_ResetBits(GPIOA, DC_Direction_Ctrl_Pin);//	

	//portB
	  GPIO_InitStructure.GPIO_Pin = LaserLED1_Pin | LaserLED2_Pin | Solenoid0_Pin | Solenoid1_Pin ;//
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
        GPIO_ResetBits(GPIOB,LaserLED1_Pin | LaserLED2_Pin |Solenoid0_Pin | Solenoid1_Pin);//

//LaserLED1_Pin, LaserLED2_Pin, LaserLED3_Pin=1 ---->, LED is on.  in the form of AC: Alternating Current
//Solenoidi_Pin = 1 ----> Solenoid is on.

	//portC
	  GPIO_InitStructure.GPIO_Pin = Solenoid2_Pin | Solenoid3_Pin | Solenoid4_Pin | Solenoid5_Pin |Solenoid6_Pin | Solenoid7_Pin | Test0_Pin;//
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
        GPIO_ResetBits(GPIOC,Solenoid2_Pin | Solenoid3_Pin | Solenoid4_Pin | Solenoid5_Pin |Solenoid6_Pin | Solenoid7_Pin | Test0_Pin);//

	//portE
	  GPIO_InitStructure.GPIO_Pin = LaserLED3_Pin | LaserLED4_Pin;//
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO
        GPIO_ResetBits(GPIOE,LaserLED3_Pin );//
        GPIO_ResetBits(GPIOE,LaserLED4_Pin);//
       
//LaserLED1_Pin, LaserLED2_Pin, LaserLED3_Pin=1 ---->, LED is on.  in the form of AC: Alternating Current
//LaserLED4_Pin=1 ---->, LED is on.  in the form of DC: Direct Current


	//portF
	  GPIO_InitStructure.GPIO_Pin =  PWMIO_Pin | BEEPIO_Pin;//
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO

	// PWMIO_Pin=0 ----> moter is on; Otherwise, PWMIO_Pin=1 ----> moter is off
        GPIO_ResetBits(GPIOF,PWMIO_Pin |BEEPIO_Pin);//

	//portG
	  GPIO_InitStructure.GPIO_Pin = HeatingRod_Pin;//
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIO
	  
	// HeatingRod_Pin=0 ----> HeatingRod is on; Otherwise, HeatingRod_Pin=1 ----> HeatingRod is off
        GPIO_SetBits(GPIOG,HeatingRod_Pin);//
        
};


