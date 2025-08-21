/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#ifndef __SSD1309_H__
#define __SSD1309_H__




void SSD1309Set_Address_x(unsigned char x);
void SSD1309_Set_Address_ypage(unsigned char page);
void SSD1309_Set_Address_x_ypage(unsigned char x,unsigned char page);
void SSD1309_Init(void);




	
	
#endif

