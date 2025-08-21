/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#ifndef __ST7796S_H__
#define __ST7796S_H__


#include "main.h"

//-------------------IC最大支持的分辨率-------------------
//#define IC_WIDTH_PIXEL (320)
//#define IC_HIGH_PIXEL  (480)
//#define IC_PIXEL  (IC_WIDTH_PIXEL*IC_HIGH_PIXEL)

void ST7796S_Set_Addr(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

void ST7796S_Set_RGB565_Mode(void);

void ST7796S_Init(void);


#endif
