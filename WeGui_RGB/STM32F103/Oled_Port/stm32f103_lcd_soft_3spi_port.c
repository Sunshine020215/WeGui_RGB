/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#include "lcd_driver_config.h"
#include "lcd_wegui_config.h"

#if (LCD_PORT == _SOFT_3SPI)


#include "stm32f103_lcd_soft_3spi_port.h"
#include "lcd_driver.h"
#include "stdint.h"



#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
static uint32_t crc[((SCREEN_HIGH+7)/8)];//储存crc
/*--------------------------------------------------------------
  * 名称: LCD_Reset_crc()
  * 功能: 刷新一次crc值(动态刷新专用)
  * 说明: 用于强制刷新屏幕,防止动态刷新出现区域不刷新
----------------------------------------------------------------*/
void LCD_Reset_crc()
{
	uint16_t i=0;
	while(i < ((SCREEN_HIGH+7)/8))
	{
		crc[i]=0xff;
		i++;
	}
}
#endif

/*--------------------------------------------------------------
  * 名称: LCD_delay_ms(volatile uint32_t ms)
  * 传入1: ms
  * 返回: 无
  * 功能: 软件延时
  * 说明: 非精准
----------------------------------------------------------------*/
void LCD_delay_ms(volatile uint32_t ms)
{
	//delay_ms(ms);
	
		volatile uint16_t i;
		while (ms--) 
		{
			i = 10000; //根据实际情况调整
			while (i--);
		}
}





#if defined (LCD_USE_NORMAL_OLED) || defined (LCD_USE_GRAY_OLED)
//-------------------------------------------以下是OLED屏幕专用驱动接口----------------------------------------------


/*--------------------------------------------------------------
  * 名称: LCD_Port_Init()
  * 传入: 无
  * 返回: 无
  * 功能: 屏幕接口初始化
  * 说明: 
----------------------------------------------------------------*/
void LCD_Port_Init(void)
{
	LCD_SCL_IO_Init();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	LCD_CS_IO_Init();
	//LCD_DC_IO_Init();//3SPI无需DC接口
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);//动态刷新CRC校验用
	#endif
	
	LCD_RES_Clr();
	{
		volatile unsigned int i=10000;
		while(i--);
	}
	LCD_RES_Set();

}
/*--------------------------------------------------------------
  * 名称: SPI_send_8bitByte(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: SPI发送1个字节数据
  * 说明: 
----------------------------------------------------------------*/
static void SPI_send_8bitByte(uint8_t dat)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{			  
		LCD_SCL_Clr();
		if(dat&0x80)
		   LCD_SDA_Set();
		else 
		   LCD_SDA_Clr();
		dat<<=1; 
		LCD_SCL_Set();
	}		  
}
/*--------------------------------------------------------------
  * 名称: LCD_Send_1Cmd(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个命令(DC=0时发1字节spi)
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Cmd(uint8_t dat)
{
	LCD_CS_Clr();
	LCD_SCL_Clr();
	LCD_SDA_Clr();
	LCD_SCL_Set();
	SPI_send_8bitByte(dat);
	LCD_CS_Set();
}
/*--------------------------------------------------------------
  * 名称: LCD_Send_1Dat(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个数据(DC=1时发1字节spi)
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Dat(uint8_t dat)
{
	LCD_CS_Clr();
	LCD_SCL_Clr();
	LCD_SDA_Set();
	LCD_SCL_Set();
	SPI_send_8bitByte(dat);
	LCD_CS_Set();
}

/*--------------------------------------------------------------
  * 名称: LCD_Send_nDat(uint8_t *p,uint16_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 返回: 无
  * 功能: 向屏幕发送num个数据
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_nDat(uint8_t *p,uint16_t num)
{
	uint8_t i=0;
	LCD_CS_Clr();

	while(num>i)
	{
		LCD_SCL_Clr();
		LCD_SDA_Set();
		LCD_SCL_Set();
		SPI_send_8bitByte(p[i++]);	  
	}
	LCD_CS_Set();
}

/*--------------------------------------------------------------
  * 名称: LCD_Send_nCmd(uint8_t *p,uint16_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 返回: 无
  * 功能: 向屏幕发送num个命令
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_nCmd(uint8_t *p,uint16_t num)
{
	uint8_t i=0;
	LCD_CS_Clr();
	while(num>i)
	{
		LCD_SCL_Clr();
		LCD_SDA_Clr();
		LCD_SCL_Set();
		SPI_send_8bitByte(p[i++]);	  
	}
	LCD_CS_Set();
}







//-------------------------------------------RGB屏幕驱动接口----------------------------------------------
#elif defined (LCD_USE_RGB565)
void LCD_Port_Init(void)
{
	LCD_SCL_IO_Init();LCD_SCL_Set();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	//LCD_DC_IO_Init();//3SPI无需DC接口
	LCD_CS_IO_Init();
	LCD_BL_IO_Init();
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);//动态刷新CRC校验用
	#endif
	
	
	LCD_RES_Clr();
	{
		volatile unsigned int i=10000;
		while(i--);
	}
	LCD_RES_Set();

}

/*--------------------------------------------------------------
  * 名称: SPI_send_8bitByte(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: SPI发送1个字节数据
  * 说明: 
----------------------------------------------------------------*/
static void SPI_send_8bitByte(uint8_t dat)
{
	//TFT SPI SCL默认常低,第一个跳变数据沿有效
	uint8_t i;
	for(i=0;i<8;i++)
	{			  
		if(dat&0x80)
		   LCD_SDA_Set();
		else 
		   LCD_SDA_Clr();
		LCD_SCL_Set();
		dat<<=1; 
		LCD_SCL_Clr();
	}		
}

/*--------------------------------------------------------------
  * 名称: LCD_Send_1Cmd(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个命令(DC=0时发1字节spi)
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Cmd(uint8_t dat)
{
	LCD_CS_Clr();
	LCD_SDA_Clr();
	LCD_SCL_Set();
	LCD_SCL_Clr();
	
	SPI_send_8bitByte(dat);
	LCD_CS_Set();
}
/*--------------------------------------------------------------
  * 名称: LCD_Send_1Dat(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个数据(DC=1时发1字节spi)
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Dat(uint8_t dat)
{
	LCD_CS_Clr();
	LCD_SDA_Set();
	LCD_SCL_Set();
	LCD_SCL_Clr();
	
	SPI_send_8bitByte(dat);
	LCD_CS_Set();
}

/*--------------------------------------------------------------
  * 名称: LCD_Send_nCmd(uint16_t *p,uint16_t num)
  * 传入1: uint16_t *p:要发送的"1位命令+n个数据"的数组起始地址
  * 传入2: uint16_t num "命令+数据"的总数量(数组的长度)
  * 返回: 无
  * 功能: 向SPI发送指令
  * 说明: 阻塞运行
----------------------------------------------------------------*/
void LCD_Send_nCmd(uint8_t *p,uint16_t num)
{
	//数组[0]按照指令发送
	//余下位按照数据发送
		
		//1.开始传输第一个命令
		LCD_CS_Clr();
		LCD_SDA_Clr();
		LCD_SCL_Set();
		LCD_SCL_Clr();
		SPI_send_8bitByte(p[0]);
		num--;
		//2.发送剩下的数据
		while(num-- > 0)
		{
			LCD_CS_Clr();
			LCD_SDA_Set();
			p++;
			LCD_SCL_Set();
			LCD_SCL_Clr();
			SPI_send_8bitByte(*p);
		}
		//5.结束
		LCD_CS_Set();	
}
#endif






/*--------------------------------------------------------------
  * 名称: LCD_Refresh(void)
  * 功能: 驱动接口,将显存LCD_GRAM全部内容发送至屏幕
----------------------------------------------------------------*/


//----------------------------普通OLED屏幕刷屏接口-------------------------------------
#if defined (LCD_USE_NORMAL_OLED)

#if (LCD_MODE == _FULL_BUFF_FULL_UPDATE)
//---------方式1:全缓存全屏刷新----------
//--优点:全屏刷新
//--缺点:内容不变的区域也参与了刷新
uint8_t LCD_Refresh(void)
{
	unsigned char ypage;
	for(ypage=0;ypage<(SCREEN_HIGH/8);ypage++)
	{
		LCD_Set_Addr(0,ypage);
		LCD_Send_nDat(&lcd_driver.LCD_GRAM[ypage][0][0],SCREEN_WIDTH);
	}
	return 0;
}

#elif (LCD_MODE == _FULL_BUFF_DYNA_UPDATE)
//---------方式2:全缓存动态刷新----------
uint8_t LCD_Refresh(void)
{
	//每page做"sum+mini_crc组合"校验,若校验码没变,则不刷新该page
	unsigned char ypage,x;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint32_t i_sum1;
		
		//-----方式1:CRC算法校验-----
		CRC->CR = CRC_CR_RESET;//CRC_ResetDR();
		for(x=0;x<SCREEN_WIDTH;x++)
		{
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][0];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x]);
		}
		i_sum1 = CRC->DR;//i_sum1 = CRC_GetCRC();
		//---------------------------

		if(crc[ypage] != i_sum1)
		{
			LCD_Set_Addr(0,ypage);
			LCD_Send_nDat(&lcd_driver.LCD_GRAM[ypage][0][0],SCREEN_WIDTH);
		}
		crc[ypage] = i_sum1;
	}
	return 0;
}
//---------方式3:页缓存全局刷新----------
#elif (LCD_MODE == _PAGE_BUFF_FULL_UPDATE)

uint8_t LCD_Refresh(void)
{
	uint8_t i=0;
	
	for(i=0;i<GRAM_YPAGE_NUM;i++)
	{
		LCD_Set_Addr(0,lcd_driver.lcd_refresh_ypage);
		LCD_Send_nDat(&lcd_driver.LCD_GRAM[i][0][0],SCREEN_WIDTH);
		lcd_driver.lcd_refresh_ypage++;
		if(lcd_driver.lcd_refresh_ypage >= ((SCREEN_HIGH+7)/8))
		{
			lcd_driver.lcd_refresh_ypage = 0;
			break;
		}
	}
	return lcd_driver.lcd_refresh_ypage;
}
//---------方式4:页缓存动态刷新----------
#elif (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE)
uint8_t LCD_Refresh(void)
{
	//每page做校验,若校验码没变,则不刷新该page
	unsigned char ypage,x;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint32_t i_crc;

		//判断屏幕是否已刷完
		if((lcd_driver.lcd_refresh_ypage + ypage)>=((SCREEN_HIGH+7)/8))
		{
			break;
		}
		//-----方式1:CRC算法校验-----
		CRC->CR = CRC_CR_RESET;//CRC_ResetDR();
		for(x=0;x<SCREEN_WIDTH;x++)
		{
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][0];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x]);
		}
		i_crc = CRC->DR;//i_sum1 = CRC_GetCRC();
		//---------------------------

		if(crc[lcd_driver.lcd_refresh_ypage + ypage] != i_crc)
		{
			LCD_Set_Addr(0,lcd_driver.lcd_refresh_ypage + ypage);
			LCD_Send_nDat(&lcd_driver.LCD_GRAM[ypage][0][0],SCREEN_WIDTH);
			crc[lcd_driver.lcd_refresh_ypage + ypage] = i_crc;
		}
		
	}
	
	lcd_driver.lcd_refresh_ypage += GRAM_YPAGE_NUM;
	if(lcd_driver.lcd_refresh_ypage >= ((SCREEN_HIGH+7)/8))
	{
		lcd_driver.lcd_refresh_ypage = 0;
	}
	return lcd_driver.lcd_refresh_ypage;
}
#endif






//----------------------------灰度OLED屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_GRAY_OLED)//灰度OLED
	//3SPI暂不支持灰度OLED屏幕
	#error("Gray OLED not sopport 3spi driver yet!Use other driver please!")





//----------------------------RGB565屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_RGB565)

/*--------------------------------------------------------------
  * 名称: LCD_Refresh(void)
  * 功能: 驱动接口,将显存LCD_GRAM全部内容发送至屏幕
----------------------------------------------------------------*/
#if (LCD_MODE == _FULL_BUFF_FULL_UPDATE)
//---------方式1:全屏从上往下刷----------
//--优点:全屏刷新
//--缺点:内容不变的区域也参与了刷新
uint8_t LCD_Refresh(void)
{
	uint16_t x,y;
	LCD_Set_Addr(0,0,SCREEN_WIDTH-1,SCREEN_HIGH-1);
	LCD_CS_Clr();
	for(y=0;y<SCREEN_HIGH;y++)
	{
		uint8_t ypage=y/8;
		uint8_t mask=0x01<<(y%8);
		uint8_t *gram=&lcd_driver.LCD_GRAM[ypage][0][0];
		for(x=0;x<SCREEN_WIDTH;x++)
		{
			uint8_t c=0;
			#if (LCD_COLOUR_BIT>=1)//---1位色---
				if(*gram++&mask){c+=1;}
			#endif
			#if (LCD_COLOUR_BIT>=2)//---2位色---
				if(*gram++&mask){c+=2;}
			#endif
			#if (LCD_COLOUR_BIT>=3)//---3位色---
				if(*gram++&mask){c+=4;}
			#endif
			LCD_SDA_Set();LCD_SCL_Set();LCD_SCL_Clr();
			SPI_send_8bitByte(lcd_driver.colour[c]>>8);
			LCD_SDA_Set();LCD_SCL_Set();LCD_SCL_Clr();
			SPI_send_8bitByte(lcd_driver.colour[c]&0xFF);
		}
	}
	LCD_CS_Set();
	return 0;
}

#elif (LCD_MODE == _FULL_BUFF_DYNA_UPDATE)
//---------方式2:动态刷新----------
//--优点:性能好,按需区域刷新,节省MCU资源
//--缺点:全屏刷新相对变慢
uint8_t LCD_Refresh(void)
{
	//每page做校验,若校验码没变,则不刷新该page
	uint8_t ypage,ycount;
	uint16_t x;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint32_t i_crc=0x0000;
		//-----方式1:CRC算法校验-----
		CRC->CR = CRC_CR_RESET;//CRC_ResetDR();
		for(x=0;x<SCREEN_WIDTH;x++)
		{
			//---1位色---
			#if (LCD_COLOUR_BIT==1)
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][0];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][0]);
			//---2位色---
			#elif (LCD_COLOUR_BIT==2)
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][0];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][0]);
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][1];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][1]);
			//---3位色---
			#elif (LCD_COLOUR_BIT==3)
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][0];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][0]);
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][1];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][1]);
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][2];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][2]);
			#else
			#error("not support this LCD_COLOUR_BIT!")
			#endif
		}
		i_crc = CRC->DR;//i_sum1 = CRC_GetCRC();
		//---------------------------
		if(crc[ypage] != i_crc)
		{
			uint8_t y=0;
			LCD_Set_Addr(0,ypage*8,SCREEN_WIDTH-1,SCREEN_HIGH-1);
			LCD_CS_Clr();
			
			ycount = 8* ypage;
			for(y=0;y<8;y++)
			{
				uint8_t x,mask;
				if(++ycount >= SCREEN_HIGH){break;}
				mask=0x01<<(y%8);
				uint8_t *gram=&lcd_driver.LCD_GRAM[ypage][0][0];
				for(x=0;x<SCREEN_WIDTH;x++)
				{
					uint8_t c=0;
					#if (LCD_COLOUR_BIT>=1)//---1位色---
						if(*gram++&mask){c+=1;}
					#endif
					#if (LCD_COLOUR_BIT>=2)//---2位色---
						if(*gram++&mask){c+=2;}
					#endif
					#if (LCD_COLOUR_BIT>=3)//---3位色---
						if(*gram++&mask){c+=4;}
					#endif
						LCD_SDA_Set();LCD_SCL_Set();LCD_SCL_Clr();
						SPI_send_8bitByte(lcd_driver.colour[c]>>8);
						LCD_SDA_Set();LCD_SCL_Set();LCD_SCL_Clr();
						SPI_send_8bitByte(lcd_driver.colour[c]&0xFF);
				}
			}
			crc[ypage] = i_crc;
		}
	}
	LCD_CS_Set();
	return 0;
}
#elif (LCD_MODE == _PAGE_BUFF_FULL_UPDATE)
//---------方式3:页缓存全屏刷新----------
uint8_t LCD_Refresh(void)
{
	uint16_t x,y;
	LCD_Set_Addr(0,lcd_driver.lcd_refresh_ypage*8,SCREEN_WIDTH-1,SCREEN_HIGH-1);

	LCD_CS_Clr();
	for(y=0;(y+lcd_driver.lcd_refresh_ypage*8)<SCREEN_HIGH;)
	{
		uint8_t mask=0x01<<(y%8);
		uint8_t *gram=&lcd_driver.LCD_GRAM[y/8][0][0];
		for(x=0;x<SCREEN_WIDTH;x++)
		{
			uint8_t c=0;
			#if (LCD_COLOUR_BIT>=1)//---1位色---
				if(*gram++&mask){c+=1;}
			#endif
			#if (LCD_COLOUR_BIT>=2)//---2位色---
				if(*gram++&mask){c+=2;}
			#endif
			#if (LCD_COLOUR_BIT>=3)//---3位色---
				if(*gram++&mask){c+=4;}
			#endif
			LCD_SDA_Set();LCD_SCL_Set();LCD_SCL_Clr();
			SPI_send_8bitByte(lcd_driver.colour[c]>>8);
			LCD_SDA_Set();LCD_SCL_Set();LCD_SCL_Clr();
			SPI_send_8bitByte(lcd_driver.colour[c]&0xFF);
		}
		y++;
		if(y>=(GRAM_YPAGE_NUM*8))
		{
			break;
		}
	}
	LCD_CS_Set();
	
	lcd_driver.lcd_refresh_ypage += GRAM_YPAGE_NUM;
	if(lcd_driver.lcd_refresh_ypage >= ((SCREEN_HIGH+7)/8))
	{
		lcd_driver.lcd_refresh_ypage = 0;
	}
	return lcd_driver.lcd_refresh_ypage;
}
#elif (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE)
//---------方式4:页缓存动态刷新----------
uint8_t LCD_Refresh(void)
{
	//每page做校验,若校验码没变,则不刷新该page
	uint8_t ypage,ycount;
	uint16_t x;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint32_t i_crc;
		if((lcd_driver.lcd_refresh_ypage + ypage)>=((SCREEN_HIGH+7)/8))
		{
			break;
		}
		//-----方式1:CRC算法校验-----
		CRC->CR = CRC_CR_RESET;//CRC_ResetDR();
		for(x=0;x<SCREEN_WIDTH;x++)
		{
			//---1位色---
			#if (LCD_COLOUR_BIT==1)
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][0];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][0]);
			//---2位色---
			#elif (LCD_COLOUR_BIT==2)
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][0];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][0]);
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][1];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][1]);
			//---3位色---
			#elif (LCD_COLOUR_BIT==3)
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][0];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][0]);
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][1];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][1]);
			CRC->DR = lcd_driver.LCD_GRAM[ypage][x][2];//CRC_CalcCRC(lcd_driver.LCD_GRAM[ypage][x][2]);
			#else
			#error("not support this LCD_COLOUR_BIT!")
			#endif
		}
		i_crc = CRC->DR;//i_sum1 = CRC_GetCRC();
		if(crc[lcd_driver.lcd_refresh_ypage + ypage] != i_crc)
		{
			uint8_t y=0;
			
			LCD_Set_Addr(0,(lcd_driver.lcd_refresh_ypage+ypage)*8,SCREEN_WIDTH-1,SCREEN_HIGH-1);
			LCD_CS_Clr();
			
			ycount = 8* (ypage + lcd_driver.lcd_refresh_ypage);
			for(y=0;y<8;y++)
			{
				uint8_t x,mask;
				if(++ycount >= SCREEN_HIGH)
				{
					break;
				}
				mask=0x01<<(y%8);
				uint8_t *gram=&lcd_driver.LCD_GRAM[ypage][0][0];
				for(x=0;x<SCREEN_WIDTH;x++)
				{
					uint8_t c=0;
					#if (LCD_COLOUR_BIT>=1)//---1位色---
						if(*gram++&mask){c+=1;}
					#endif
					#if (LCD_COLOUR_BIT>=2)//---2位色---
						if(*gram++&mask){c+=2;}
					#endif
					#if (LCD_COLOUR_BIT>=3)//---3位色---
						if(*gram++&mask){c+=4;}
					#endif
					LCD_SDA_Set();LCD_SCL_Set();LCD_SCL_Clr();
					SPI_send_8bitByte(lcd_driver.colour[c]>>8);
					LCD_SDA_Set();LCD_SCL_Set();LCD_SCL_Clr();
					SPI_send_8bitByte(lcd_driver.colour[c]&0xFF);
				}
			}
			crc[lcd_driver.lcd_refresh_ypage + ypage] = i_crc;
			LCD_CS_Set();
		}
	}
	lcd_driver.lcd_refresh_ypage += GRAM_YPAGE_NUM;
	if(lcd_driver.lcd_refresh_ypage >= ((SCREEN_HIGH+7)/8))
	{
		lcd_driver.lcd_refresh_ypage = 0;
	}
	return lcd_driver.lcd_refresh_ypage;
}
#endif
#endif







#endif





