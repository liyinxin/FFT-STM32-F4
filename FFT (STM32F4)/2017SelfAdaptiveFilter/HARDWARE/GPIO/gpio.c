#include "gpio.h"


void gpioInitiation(void)
	{

	  GPIO_InitTypeDef  GPIO_InitStructure;

//	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//
//	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//
//	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//
//	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//
       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//
//	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//

//	//portA
//	  GPIO_InitStructure.GPIO_Pin = RS1_DIR_GPIO_PIN | RS2_DIR_GPIO_PIN | RS3_DIR_GPIO_PIN;//
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

//         GPIO_ResetBits(GPIOA, RS1_DIR_GPIO_PIN);//
//         GPIO_SetBits(GPIOA, RS2_DIR_GPIO_PIN);//
//         GPIO_ResetBits(GPIOA, RS3_DIR_GPIO_PIN);//


//	//portB
//	  GPIO_InitStructure.GPIO_Pin = RS4_DIR_GPIO_PIN | RS5_DIR_GPIO_PIN ;//
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
//         GPIO_ResetBits(GPIOB,RS4_DIR_GPIO_PIN | RS5_DIR_GPIO_PIN);//
//		 
//	 
//	  GPIO_InitStructure.GPIO_Pin = CPU_ID1_GPIO_PIN | CPU_ID2_GPIO_PIN;//
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输出模式
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO		 
////         GPIO_SetBits(GPIOB,CPU_ID1_GPIO_PIN | CPU_ID2_GPIO_PIN);//

//	//portC
//	  GPIO_InitStructure.GPIO_Pin = CPU_EN1_GPIO_PIN | CPU_EN2_GPIO_PIN | CPU_EN3_GPIO_PIN | CPU_EN4_GPIO_PIN |RS1_LED_GPIO_PIN;//
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
//         GPIO_ResetBits(GPIOC,CPU_EN1_GPIO_PIN | CPU_EN2_GPIO_PIN | CPU_EN3_GPIO_PIN | CPU_EN4_GPIO_PIN );//
//         GPIO_SetBits(GPIOC,RS1_LED_GPIO_PIN);
//		 
//	//portD
//	  GPIO_InitStructure.GPIO_Pin = RS2_LED_GPIO_PIN | RS3_LED_GPIO_PIN | RS4_LED_GPIO_PIN;//
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
//         GPIO_SetBits(GPIOD,RS2_LED_GPIO_PIN | RS3_LED_GPIO_PIN | RS4_LED_GPIO_PIN);//

	//portE
	  GPIO_InitStructure.GPIO_Pin = WipeUpDown_Pin | Pulse_Pin;//
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输入模式
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO
         GPIO_SetBits(GPIOE,WipeUpDown_Pin );//
         GPIO_SetBits(GPIOE,Pulse_Pin);//

	//portF
	  GPIO_InitStructure.GPIO_Pin =  TestGPIO0_Pin | TestGPIO1_Pin | TestGPIO2_Pin | WipeUpDown_Pin | Pulse_Pin;//
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
    	  GPIO_SetBits(GPIOF, TestGPIO0_Pin | TestGPIO1_Pin | TestGPIO2_Pin |WipeUpDown_Pin | Pulse_Pin);//

//	//portG
//	  GPIO_InitStructure.GPIO_Pin = MOD1_LED_GPIO_PIN |MOD2_LED_GPIO_PIN |MOD3_LED_GPIO_PIN |MOD4_LED_GPIO_PIN |MOD5_LED_GPIO_PIN;//
//	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//	  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIO
//    GPIO_SetBits(GPIOG, MOD1_LED_GPIO_PIN |MOD2_LED_GPIO_PIN |MOD3_LED_GPIO_PIN |MOD4_LED_GPIO_PIN |MOD5_LED_GPIO_PIN);//

		
};


