/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#ifndef __SSD1306_H__
#define __SSD1306_H__




void SSD1306Set_Address_x(unsigned char x);
void SSD1306_Set_Address_ypage(unsigned char page);
void SSD1306_Set_Address_x_ypage(unsigned char x,unsigned char page);
void SSD1306_Init(void);




	
	
#endif

