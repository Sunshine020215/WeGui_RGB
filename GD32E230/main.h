#ifndef __MAIN_H__
#define __MAIN_H__

#include "gd32e23x.h"
#include "gd32e23x_it.h"
#include "systick.h"
#include "lcd_driver.h"
#include "lcd_wegui_driver.h"



#define ResADC_GPIOx                 GPIOA
#define ResADC_GPIO_PIN_x            GPIO_PIN_1
#define ResADC_RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOA
#define ResADC_IO_Init()                                        \
do                                                                  \
{                                                                   \
    GPIO_InitTypeDef GPIO_InitStruct;                               \
    GPIO_InitStruct.GPIO_Pin   = ResADC_GPIO_PIN_x;             \
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;                   \
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;                     \
	  RCC_APB2PeriphClockCmd(ResADC_RCC_APB2Periph_GPIOx,ENABLE); \
    GPIO_Init(ResADC_GPIOx, &GPIO_InitStruct);                  \
}while(0)
#endif
