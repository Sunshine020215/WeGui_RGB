/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/
#include "lcd_driver_config.h"

#if (LCD_IC == _ST7789V3)
#include "st7789v3.h"





void ST7789V3_Set_Addr(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	uint8_t i[]={0x2a,x1>>8,x1&0xff,(x2)>>8,(x2)&0xff};
	uint8_t j[]={0x2b,y1>>8,(y1)&0xff,(y2)>>8,y2&0xff};
	const uint8_t k[]={0x2c};
	LCD_Send_nCmd((uint8_t*)i,sizeof(i)/sizeof(uint8_t));
	LCD_Send_nCmd((uint8_t*)j,sizeof(j)/sizeof(uint8_t));
	LCD_Send_nCmd((uint8_t*)k,sizeof(k)/sizeof(uint8_t));
}

void ST7789V3_Clear()//清除IC显示缓存
{
	uint32_t i;
	ST7789V3_Set_Addr(0,0,320-1,320-1);
	i=0;
	while(i++<(uint32_t)320*320)
	{
		LCD_Send_1Dat(0x33);LCD_Send_1Dat(0x33);//测试
		//LCD_Send_1Dat(0x00);LCD_Send_1Dat(0x00);
	}
}



void ST7789V3_Init()
{
	LCD_RES_Clr();
	LCD_delay_ms(100);
	LCD_RES_Set();
	LCD_delay_ms(100);

	LCD_Send_1Cmd(0x11); 
//	delay_1ms(120); 
	LCD_Send_1Cmd(0x36); 
	//LCD_Send_1Dat(0x00);//方向1
	//LCD_Send_1Dat(0xC0);//方向2
	LCD_Send_1Dat(0x70);//方向3
	//LCD_Send_1Dat(0xA0);//方向4

	LCD_Send_1Cmd(0x3A);
	LCD_Send_1Dat(0x05);

	LCD_Send_1Cmd(0xB2);
	LCD_Send_1Dat(0x0C);
	LCD_Send_1Dat(0x0C);
	LCD_Send_1Dat(0x00);
	LCD_Send_1Dat(0x33);
	LCD_Send_1Dat(0x33); 

	LCD_Send_1Cmd(0xB7); 
	LCD_Send_1Dat(0x35);  

	LCD_Send_1Cmd(0xBB);
	LCD_Send_1Dat(0x35);

	LCD_Send_1Cmd(0xC0);
	LCD_Send_1Dat(0x2C);

	LCD_Send_1Cmd(0xC2);
	LCD_Send_1Dat(0x01);

	LCD_Send_1Cmd(0xC3);
	LCD_Send_1Dat(0x13);   

	LCD_Send_1Cmd(0xC4);
	LCD_Send_1Dat(0x20);  

	LCD_Send_1Cmd(0xC6); 
	LCD_Send_1Dat(0x0F);    

	LCD_Send_1Cmd(0xD0); 
	LCD_Send_1Dat(0xA4);
	LCD_Send_1Dat(0xA1);

	LCD_Send_1Cmd(0xD6); 
	LCD_Send_1Dat(0xA1);

	LCD_Send_1Cmd(0xE0);
	LCD_Send_1Dat(0xF0);
	LCD_Send_1Dat(0x00);
	LCD_Send_1Dat(0x04);
	LCD_Send_1Dat(0x04);
	LCD_Send_1Dat(0x04);
	LCD_Send_1Dat(0x05);
	LCD_Send_1Dat(0x29);
	LCD_Send_1Dat(0x33);
	LCD_Send_1Dat(0x3E);
	LCD_Send_1Dat(0x38);
	LCD_Send_1Dat(0x12);
	LCD_Send_1Dat(0x12);
	LCD_Send_1Dat(0x28);
	LCD_Send_1Dat(0x30);

	LCD_Send_1Cmd(0xE1);
	LCD_Send_1Dat(0xF0);
	LCD_Send_1Dat(0x07);
	LCD_Send_1Dat(0x0A);
	LCD_Send_1Dat(0x0D);
	LCD_Send_1Dat(0x0B);
	LCD_Send_1Dat(0x07);
	LCD_Send_1Dat(0x28);
	LCD_Send_1Dat(0x33);
	LCD_Send_1Dat(0x3E);
	LCD_Send_1Dat(0x36);
	LCD_Send_1Dat(0x14);
	LCD_Send_1Dat(0x14);
	LCD_Send_1Dat(0x29);
	LCD_Send_1Dat(0x32);
	
// 	LCD_Send_1Cmd(0x2A);
//	LCD_Send_1Dat(0x00);
//	LCD_Send_1Dat(0x22);
//	LCD_Send_1Dat(0x00);
//	LCD_Send_1Dat(0xCD);
//	LCD_Send_1Dat(0x2B);
//	LCD_Send_1Dat(0x00);
//	LCD_Send_1Dat(0x00);
//	LCD_Send_1Dat(0x01);
//	LCD_Send_1Dat(0x3F);
//	LCD_Send_1Cmd(0x2C);
	LCD_Send_1Cmd(0x21); 
  
  LCD_Send_1Cmd(0x11);
  LCD_delay_ms(120);	
	LCD_Send_1Cmd(0x29); 
	ST7789V3_Clear();//清除IC显示缓存
}
#endif
