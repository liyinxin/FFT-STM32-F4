#ifndef __GPIO_H__
#define  __GPIO_H__

//#include "sys.h"
#include "stm32f4xx_gpio.h"

///*PortA*/
//#define	Sampling_ChannelSel	PAout(6)
//#define Sampling_ChannelSel_Pin  GPIO_Pin_6


////#define SamplingAd630Sel_A_Port GPIOB
//#define	SamplingAd630Sel_A	PAout(7)
//#define SamplingAd630Sel_A_Pin  GPIO_Pin_7

////#define SamplingAd630Sel_B_Port GPIOB
//#define	SamplingAd630Sel_B	PAout(8)
//#define SamplingAd630Sel_B_Pin  GPIO_Pin_8

//#define DC_Direction_Ctrl PAout(1)
//#define DC_Direction_Ctrl_Pin GPIO_Pin_1


///*PortB*/
//#define LaserLED1   PBout(6)
//#define	LaserLED1_Pin	GPIO_Pin_6

//#define LaserLED2   PBout(7)
//#define	LaserLED2_Pin	GPIO_Pin_7

//#define Solenoid0 PBout(0)
//#define Solenoid0_Pin GPIO_Pin_0

//#define Solenoid1 PBout(1)
//#define Solenoid1_Pin GPIO_Pin_1

///*PortC*/
//#define Solenoid2 PCout(1)
//#define Solenoid2_Pin GPIO_Pin_1

//#define Solenoid3 PCout(2)
//#define Solenoid3_Pin GPIO_Pin_2

//#define Solenoid4 PCout(3)
//#define Solenoid4_Pin GPIO_Pin_3

//#define Solenoid5 PCout(4)
//#define Solenoid5_Pin GPIO_Pin_4

//#define Solenoid6 PCout(5)
//#define Solenoid6_Pin GPIO_Pin_5

//#define Solenoid7 PCout(6)
//#define Solenoid7_Pin GPIO_Pin_6

//#define Test0 PCout(13)
//#define Test0_Pin GPIO_Pin_13

///*PortE*/
//#define LaserLED3   PEout(5)
//#define	LaserLED3_Pin	GPIO_Pin_5

//#define LaserLED4   PEout(6)
//#define	LaserLED4_Pin	GPIO_Pin_6

///*PortF*/
//#define BEEPIO   PFout(8)
//#define	BEEPIO_Pin	GPIO_Pin_8
//#define PWMIO   PFout(9)
//#define	PWMIO_Pin	GPIO_Pin_9
#define TestGPIO0   PFout(0)
#define	TestGPIO0_Pin	GPIO_Pin_0
#define TestGPIO1   PFout(1)
#define	TestGPIO1_Pin	GPIO_Pin_1
#define TestGPIO2   PFout(2)
#define	TestGPIO2_Pin	GPIO_Pin_2



///*PortG*/
//#define HeatingRod   PGout(9)
//#define	HeatingRod_Pin	GPIO_Pin_9





//Initialize some gpio for the application*******************
void gpioAppInitiation(void);

#endif
