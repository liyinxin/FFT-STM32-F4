#ifndef __GPIO_H__
#define  __GPIO_H__

//#include "sys.h"
#include "stm32f4xx_gpio.h"

///*PortA*/
//#define	RS1_DIR_GPIO	PAout(6)
//#define RS1_DIR_GPIO_PIN  GPIO_Pin_6

//#define	RS2_DIR_GPIO	PAout(7)
//#define RS2_DIR_GPIO_PIN  GPIO_Pin_7

//#define	RS3_DIR_GPIO	PAout(8)
//#define RS3_DIR_GPIO_PIN  GPIO_Pin_8

///*PortB*/

//#define RS4_DIR_GPIO PBout(0)
//#define RS4_DIR_GPIO_PIN GPIO_Pin_0

//#define RS5_DIR_GPIO PBout(1)
//#define RS5_DIR_GPIO_PIN GPIO_Pin_1

//#define CPU_ID1_GPIO   PBin(6)////////输入口
//#define CPU_ID1_GPIO_PIN	GPIO_Pin_6

//#define CPU_ID2_GPIO   PBin(7)////////输入口
//#define CPU_ID2_GPIO_PIN	GPIO_Pin_7

///*PortC*/
//#define CPU_EN1_GPIO PCout(1)
//#define CPU_EN1_GPIO_PIN GPIO_Pin_1

//#define CPU_EN2_GPIO PCout(2)
//#define CPU_EN2_GPIO_PIN GPIO_Pin_2

//#define CPU_EN3_GPIO PCout(3)
//#define CPU_EN3_GPIO_PIN GPIO_Pin_3

//#define CPU_EN4_GPIO PCout(4)
//#define CPU_EN4_GPIO_PIN GPIO_Pin_4

//#define RS1_LED_GPIO PCout(5)
//#define RS1_LED_GPIO_PIN GPIO_Pin_5

///*PortD*/
//#define RS2_LED_GPIO PDout(3)
//#define RS2_LED_GPIO_PIN GPIO_Pin_3

//#define RS3_LED_GPIO PDout(6)
//#define RS3_LED_GPIO_PIN GPIO_Pin_6

//#define RS4_LED_GPIO PDout(7)
//#define RS4_LED_GPIO_PIN GPIO_Pin_7


///*PortE*/
//#define CPU_ID3_GPIO   PEin(5)////////输入口
//#define CPU_ID3_GPIO_PIN	GPIO_Pin_5

//#define CPU_ID4_GPIO   PEin(6)////////输入口
//#define CPU_ID4_GPIO_PIN	GPIO_Pin_6
#define WipeUpDownGPIO  PEout(14)
#define	WipeUpDown_Pin	GPIO_Pin_14
#define PulseGPIO   PEout(15)
#define	Pulse_Pin	GPIO_Pin_15


///*PortF*/
//#define BEEPIO   PFout(8)
//#define BEEPIO_Pin	GPIO_Pin_8
//#define RS5_LED_GPIO   PFout(9)
//#define RS5_LED_GPIO_PIN	GPIO_Pin_9
#define TestGPIO0   PFout(0)
#define	TestGPIO0_Pin	GPIO_Pin_0
#define TestGPIO1   PFout(1)
#define	TestGPIO1_Pin	GPIO_Pin_1
#define TestGPIO2   PFout(2)
#define	TestGPIO2_Pin	GPIO_Pin_2



///*PortG*/
//#define MOD1_LED_GPIO   PGout(6)
//#define MOD1_LED_GPIO_PIN GPIO_Pin_6

//#define MOD2_LED_GPIO   PGout(7)
//#define MOD2_LED_GPIO_PIN GPIO_Pin_7

//#define MOD3_LED_GPIO   PGout(9)
//#define MOD3_LED_GPIO_PIN GPIO_Pin_9

//#define MOD4_LED_GPIO   PGout(13)
//#define MOD4_LED_GPIO_PIN GPIO_Pin_13

//#define MOD5_LED_GPIO   PGout(14)
//#define MOD5_LED_GPIO_PIN GPIO_Pin_14

//Initialize some gpio for the application*******************
void gpioInitiation(void);

#endif
