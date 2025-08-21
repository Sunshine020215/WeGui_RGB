/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#ifndef __ST7789V3_H__
#define __ST7789V3_H__

#include "stdint.h"

//-------------------IC最大支持的分辨率-------------------
//#define TFT_IC_WIDTH_PIXEL (320)
//#define TFT_IC_HIGH_PIXEL  (240)
//#define TFT_IC_PIXEL  (TFT_IC_WIDTH_PIXEL*TFT_IC_HIGH_PIXEL)

void ST7789V3_Set_Addr(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void ST7789V3_Init(void);


#endif
