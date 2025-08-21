/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#ifndef _STM32F103_Wegui_4KEY_PORT_H_
#define _STM32F103_Wegui_4KEY_PORT_H_

#include "gd32e23x.h"
#include "lcd_wegui_driver.h"










//-------------------------------------按键配置-------------------------------------

#define Wegui_KeyBack_GPIOx      GPIOC
#define Wegui_KeyBack_GPIO_PIN_x GPIO_PIN_15
#define Wegui_KeyBack_RCU_GPIOx  RCU_GPIOC
#define Wegui_KeyBack_IO_Init()                                                                   \
do                                                                                                \
{                                                                                                 \
	rcu_periph_clock_enable(Wegui_KeyBack_RCU_GPIOx);                                                   \
	gpio_mode_set(Wegui_KeyBack_GPIOx, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Wegui_KeyBack_GPIO_PIN_x);\
}while(0)

#define Wegui_KeyOk_GPIOx      GPIOA
#define Wegui_KeyOk_GPIO_PIN_x GPIO_PIN_0
#define Wegui_KeyOk_RCU_GPIOx  RCU_GPIOA
#define Wegui_KeyOk_IO_Init()                                                                 \
do                                                                                            \
{                                                                                             \
	rcu_periph_clock_enable(Wegui_KeyOk_RCU_GPIOx);                                                 \
	gpio_mode_set(Wegui_KeyOk_GPIOx, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Wegui_KeyOk_GPIO_PIN_x);\
}while(0)



									
#define Wegui_KeyRight_GPIOx      GPIOB
#define Wegui_KeyRight_GPIO_PIN_x GPIO_PIN_12
#define Wegui_KeyRight_RCU_GPIOx  RCU_GPIOB
#define Wegui_KeyRight_IO_Init()                                                                    \
do                                                                                                  \
{                                                                                                   \
	rcu_periph_clock_enable(Wegui_KeyRight_RCU_GPIOx);                                                    \
	gpio_mode_set(Wegui_KeyRight_GPIOx, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Wegui_KeyRight_GPIO_PIN_x);\
}while(0)

#define Wegui_KeyUp_GPIOx      GPIOB
#define Wegui_KeyUp_GPIO_PIN_x GPIO_PIN_13
#define Wegui_KeyUp_RCU_GPIOx  RCU_GPIOB
#define Wegui_KeyUp_IO_Init()                                                                 \
do                                                                                            \
{                                                                                             \
	rcu_periph_clock_enable(Wegui_KeyUp_RCU_GPIOx);                                                 \
	gpio_mode_set(Wegui_KeyUp_GPIOx, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Wegui_KeyUp_GPIO_PIN_x);\
}while(0)

#define Wegui_KeyDown_GPIOx      GPIOB
#define Wegui_KeyDown_GPIO_PIN_x GPIO_PIN_14
#define Wegui_KeyDown_RCU_GPIOx  RCU_GPIOB
#define Wegui_KeyDown_IO_Init()                                                                   \
do                                                                                                \
{                                                                                                 \
	rcu_periph_clock_enable(Wegui_KeyDown_RCU_GPIOx);                                                   \
	gpio_mode_set(Wegui_KeyDown_GPIOx, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Wegui_KeyDown_GPIO_PIN_x);\
}while(0)

#define Wegui_KeyLeft_GPIOx      GPIOB
#define Wegui_KeyLeft_GPIO_PIN_x GPIO_PIN_15
#define Wegui_KeyLeft_RCU_GPIOx  RCU_GPIOB
#define Wegui_KeyLeft_IO_Init()                                                                   \
do                                                                                                \
{                                                                                                 \
	rcu_periph_clock_enable(Wegui_KeyLeft_RCU_GPIOx);                                                   \
	gpio_mode_set(Wegui_KeyLeft_GPIOx, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, Wegui_KeyLeft_GPIO_PIN_x);\
}while(0)





//开发板正向按键
/*---------------------------
	*  ---默认按键接口---
	*  |OK   | A0  |确定|
	*  |BACK | C15 |返回|
	*  |UP   | B13 | 上 |
	*  |DOWN | B14 | 下 |
	*  |LEFT | B15 | 左 |
	*  |RIGHT| B12 | 右 |
----------------------------*/
//#define Bool_WKeyBack_is_Pressed()  (gpio_input_bit_get(Wegui_KeyBack_GPIOx, Wegui_KeyBack_GPIO_PIN_x) == RESET)
//#define Bool_WKeyOk_is_Pressed()    (gpio_input_bit_get(Wegui_KeyOk_GPIOx, Wegui_KeyOk_GPIO_PIN_x) == RESET)
#define Bool_WkeyRight_is_Pressed() (gpio_input_bit_get(Wegui_KeyRight_GPIOx, Wegui_KeyRight_GPIO_PIN_x) == RESET)
#define Bool_WKeyUp_is_Pressed()    (gpio_input_bit_get(Wegui_KeyUp_GPIOx, Wegui_KeyUp_GPIO_PIN_x) == RESET)
#define Bool_WKeyDown_is_Pressed()  (gpio_input_bit_get(Wegui_KeyDown_GPIOx, Wegui_KeyDown_GPIO_PIN_x) == RESET)
#define Bool_WkeyLeft_is_Pressed()  (gpio_input_bit_get(Wegui_KeyLeft_GPIOx, Wegui_KeyLeft_GPIO_PIN_x) == RESET)





//-------------------------------------声明-------------------------------------
void Wegui_4key_port_Init(void);
void Wegui_Interface_stick(uint16_t ms);


#endif
