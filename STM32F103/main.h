/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "lcd_driver.h"
#include "lcd_Wegui_driver.h"

void delay_ms(uint32_t ms);




#define ResADC_GPIOx                 GPIOA
#define ResADC_GPIO_Pin_x            GPIO_Pin_1
#define ResADC_RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOA
#define ResADC_IO_Init()                                        \
do                                                                  \
{                                                                   \
    GPIO_InitTypeDef GPIO_InitStruct;                               \
    GPIO_InitStruct.GPIO_Pin   = ResADC_GPIO_Pin_x;             \
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;                   \
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;                     \
	  RCC_APB2PeriphClockCmd(ResADC_RCC_APB2Periph_GPIOx,ENABLE); \
    GPIO_Init(ResADC_GPIOx, &GPIO_InitStruct);                  \
}while(0)
#endif
