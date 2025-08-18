/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#include <stdint.h>
#include "lcd_driver.h"


//---------驱动结构体---------
lcd_driver_t lcd_driver;

//---------快速计算用---------
const static uint8_t cal_1[] = {0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF,0x57,0x65,0x47,0x75,0x69,0x20,0x52,0x47,0x42};//用于快速计算


//--------------------------------------------------------------驱动接口--------------------------------------------------------------

	
	#if (defined LCD_USE_RGB565)
/*--------------------------------------------------------------
  * 名称: RGB_Set_Driver_Colour(uint8_t num,uint16_t colour)
	* 传入: num 颜色序号 对应设置writer_num的笔刷颜色
	* 传入: colour 颜色
	* 功能: 设置驱动笔刷颜色
  * 说明: 
----------------------------------------------------------------*/
void RGB_Set_Driver_Colour(uint8_t num,uint16_t colour)
{
		#if (LCD_COLOUR_BIT == 1)
		static uint16_t colour_lastime[2]={0x0000,0x0000};
		#elif (LCD_COLOUR_BIT == 2)
		static uint16_t colour_lastime[4]={0x0000,0x0000,0x0000,0x0000};
		#elif (LCD_COLOUR_BIT == 3)
		static uint16_t colour_lastime[8]={0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
		#else
		#error("not support LCD_COLOUR_BIT!")
		#endif
		
		if(colour_lastime[num] != colour)
		{
			lcd_driver.colour[num] = colour_lastime[num] = colour;
			#if((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
			//动态刷新才有crc
			LCD_Reset_crc();//色彩已改变,强制刷新一下crc动态刷新值, 保证颜色正确
			#endif
		}
}
#endif


/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_0(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 普通快速驱动函数,将值以0的颜色写入显存
----------------------------------------------------------------*/
static void lcd_driver_Write_0(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
	
#		if (LCD_COLOUR_BIT==1)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((ypage >= lcd_driver.lcd_refresh_ypage) && (ypage <= lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM-1))
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==1)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}


/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_1(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 普通快速驱动函数,将值以1的颜色写入显存
----------------------------------------------------------------*/
static void lcd_driver_Write_1(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==1)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((ypage >= lcd_driver.lcd_refresh_ypage) && (ypage <= lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM-1))
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==1)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_2(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 普通快速驱动函数,将值以1的颜色写入显存
----------------------------------------------------------------*/
#if(LCD_COLOUR_BIT>=2)
static void lcd_driver_Write_2(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] &=(~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &=(~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((ypage >= lcd_driver.lcd_refresh_ypage) && (ypage <= lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM-1))
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] &=(~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &=(~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif


/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_3(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 普通快速驱动函数,将值以3的颜色写入显存
----------------------------------------------------------------*/
#if(LCD_COLOUR_BIT>=2)
static void lcd_driver_Write_3(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((ypage >= lcd_driver.lcd_refresh_ypage) && (ypage <= lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM-1))
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_4(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 普通快速驱动函数,将值以4的颜色写入显存
----------------------------------------------------------------*/
#if(LCD_COLOUR_BIT>=3)
static void lcd_driver_Write_4(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((ypage >= lcd_driver.lcd_refresh_ypage) && (ypage <= lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM-1))
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_5(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 普通快速驱动函数,将值以5的颜色写入显存
----------------------------------------------------------------*/
#if(LCD_COLOUR_BIT>=3)
static void lcd_driver_Write_5(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((ypage >= lcd_driver.lcd_refresh_ypage) && (ypage <= lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM-1))
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0]|= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_6(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 普通快速驱动函数,将值以6的颜色写入显存
----------------------------------------------------------------*/
#if(LCD_COLOUR_BIT>=3)
static void lcd_driver_Write_6(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((ypage >= lcd_driver.lcd_refresh_ypage) && (ypage <= lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM-1))
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_7(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 普通快速驱动函数,将值以7的颜色写入显存
----------------------------------------------------------------*/
#if(LCD_COLOUR_BIT>=3)
static void lcd_driver_Write_7(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((ypage >= lcd_driver.lcd_refresh_ypage) && (ypage <= lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM-1))
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_inv(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 普通快速驱动函数,将值以"反色"的方式写入显存
----------------------------------------------------------------*/
static void lcd_driver_Write_inv(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==1)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] ^= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] ^= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((ypage >= lcd_driver.lcd_refresh_ypage) && (ypage <= lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM-1))
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==1)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] ^= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] ^= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_0_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 高级驱动函数,在限制区域(Box)内,将值以0的颜色写入到显存
----------------------------------------------------------------*/
static void lcd_driver_Write_0_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	if((x<lcd_driver.Box.x_min)||(x>lcd_driver.Box.x_max))
		return;
	if((ypage<lcd_driver.Box.ypage_min)||(ypage>lcd_driver.Box.ypage_max))
		return;
	if(ypage == lcd_driver.Box.ypage_min_temp)
		u8_value = u8_value & lcd_driver.Box.ypage_min_temp;
	if(ypage == lcd_driver.Box.ypage_max_temp)
		u8_value = u8_value & lcd_driver.Box.ypage_max_temp;
	
	
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==1)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((ypage >= lcd_driver.lcd_refresh_ypage) && (ypage <= lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM-1))
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==1)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_1_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 高级驱动函数,在限制区域(Box)内,将值以1的颜色写入到显存
----------------------------------------------------------------*/
static void lcd_driver_Write_1_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	if((x<lcd_driver.Box.x_min)||(x>lcd_driver.Box.x_max))
		return;
	if((ypage<lcd_driver.Box.ypage_min)||(ypage>lcd_driver.Box.ypage_max))
		return;
	
	if(ypage == lcd_driver.Box.ypage_min)
		u8_value = u8_value & lcd_driver.Box.ypage_min_temp;
	if(ypage == lcd_driver.Box.ypage_max)
		u8_value = u8_value & lcd_driver.Box.ypage_max_temp;
	if(u8_value == 0x00)
		return;
	
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==1)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if(ypage == lcd_driver.lcd_refresh_ypage)
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==1)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}


/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_2_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 高级驱动函数,在限制区域(Box)内,将值以2的颜色写入到显存
----------------------------------------------------------------*/
#if (LCD_COLOUR_BIT>=2)
static void lcd_driver_Write_2_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	if((x<lcd_driver.Box.x_min)||(x>lcd_driver.Box.x_max))
		return;
	if((ypage<lcd_driver.Box.ypage_min)||(ypage>lcd_driver.Box.ypage_max))
		return;
	
	if(ypage == lcd_driver.Box.ypage_min)
		u8_value = u8_value & lcd_driver.Box.ypage_min_temp;
	if(ypage == lcd_driver.Box.ypage_max)
		u8_value = u8_value & lcd_driver.Box.ypage_max_temp;
	if(u8_value == 0x00)
		return;
	
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if(ypage == lcd_driver.lcd_refresh_ypage)
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif


/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_3_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 高级驱动函数,在限制区域(Box)内,将值以3的颜色写入到显存
----------------------------------------------------------------*/
#if (LCD_COLOUR_BIT>=2)
static void lcd_driver_Write_3_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	if((x<lcd_driver.Box.x_min)||(x>lcd_driver.Box.x_max))
		return;
	if((ypage<lcd_driver.Box.ypage_min)||(ypage>lcd_driver.Box.ypage_max))
		return;
	
	if(ypage == lcd_driver.Box.ypage_min)
		u8_value = u8_value & lcd_driver.Box.ypage_min_temp;
	if(ypage == lcd_driver.Box.ypage_max)
		u8_value = u8_value & lcd_driver.Box.ypage_max_temp;
	if(u8_value == 0x00)
		return;
	
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------单页缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if(ypage == lcd_driver.lcd_refresh_ypage)
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] &= (~u8_value);
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif


/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_4_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 高级驱动函数,在限制区域(Box)内,将值以4的颜色写入到显存
----------------------------------------------------------------*/
#if (LCD_COLOUR_BIT>=3)
static void lcd_driver_Write_4_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	if((x<lcd_driver.Box.x_min)||(x>lcd_driver.Box.x_max))
		return;
	if((ypage<lcd_driver.Box.ypage_min)||(ypage>lcd_driver.Box.ypage_max))
		return;
	
	if(ypage == lcd_driver.Box.ypage_min)
		u8_value = u8_value & lcd_driver.Box.ypage_min_temp;
	if(ypage == lcd_driver.Box.ypage_max)
		u8_value = u8_value & lcd_driver.Box.ypage_max_temp;
	if(u8_value == 0x00)
		return;
	
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------单页缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if(ypage == lcd_driver.lcd_refresh_ypage)
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_5_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 高级驱动函数,在限制区域(Box)内,将值以5的颜色写入到显存
----------------------------------------------------------------*/
#if (LCD_COLOUR_BIT>=3)
static void lcd_driver_Write_5_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	if((x<lcd_driver.Box.x_min)||(x>lcd_driver.Box.x_max))
		return;
	if((ypage<lcd_driver.Box.ypage_min)||(ypage>lcd_driver.Box.ypage_max))
		return;
	
	if(ypage == lcd_driver.Box.ypage_min)
		u8_value = u8_value & lcd_driver.Box.ypage_min_temp;
	if(ypage == lcd_driver.Box.ypage_max)
		u8_value = u8_value & lcd_driver.Box.ypage_max_temp;
	if(u8_value == 0x00)
		return;
	
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------单页缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if(ypage == lcd_driver.lcd_refresh_ypage)
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_6_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 高级驱动函数,在限制区域(Box)内,将值以6的颜色写入到显存
----------------------------------------------------------------*/
#if (LCD_COLOUR_BIT>=3)
static void lcd_driver_Write_6_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	if((x<lcd_driver.Box.x_min)||(x>lcd_driver.Box.x_max))
		return;
	if((ypage<lcd_driver.Box.ypage_min)||(ypage>lcd_driver.Box.ypage_max))
		return;
	
	if(ypage == lcd_driver.Box.ypage_min)
		u8_value = u8_value & lcd_driver.Box.ypage_min_temp;
	if(ypage == lcd_driver.Box.ypage_max)
		u8_value = u8_value & lcd_driver.Box.ypage_max_temp;
	if(u8_value == 0x00)
		return;
	
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------单页缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if(ypage == lcd_driver.lcd_refresh_ypage)
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] &= (~u8_value);
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_7_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 高级驱动函数,在限制区域(Box)内,将值以7的颜色写入到显存
----------------------------------------------------------------*/
#if (LCD_COLOUR_BIT>=3)
static void lcd_driver_Write_7_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	if((x<lcd_driver.Box.x_min)||(x>lcd_driver.Box.x_max))
		return;
	if((ypage<lcd_driver.Box.ypage_min)||(ypage>lcd_driver.Box.ypage_max))
		return;
	
	if(ypage == lcd_driver.Box.ypage_min)
		u8_value = u8_value & lcd_driver.Box.ypage_min_temp;
	if(ypage == lcd_driver.Box.ypage_max)
		u8_value = u8_value & lcd_driver.Box.ypage_max_temp;
	if(u8_value == 0x00)
		return;
	
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if(ypage == lcd_driver.lcd_refresh_ypage)
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] |= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] |= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}
#endif

/*--------------------------------------------------------------
  * 名称: lcd_driver_Write_inv_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
  * 功能: 高级驱动函数,在限制区域(Box)内,将值以反写的方式写入到显存
----------------------------------------------------------------*/
static void lcd_driver_Write_inv_inBox(uint16_t x,uint16_t ypage,uint8_t u8_value)
{
	if((x<lcd_driver.Box.x_min)||(x>lcd_driver.Box.x_max))
		return;
	if((ypage<lcd_driver.Box.ypage_min)||(ypage>lcd_driver.Box.ypage_max))
		return;
	
	if(ypage == lcd_driver.Box.ypage_min)
		u8_value = u8_value & lcd_driver.Box.ypage_min_temp;
	if(ypage == lcd_driver.Box.ypage_max)
		u8_value = u8_value & lcd_driver.Box.ypage_max_temp;
	if(u8_value == 0x00)
		return;
	
	
	//---------全屏缓存-----------
#	if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
#		if (LCD_COLOUR_BIT>=1)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] ^= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] ^= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	//---------自定缓存-----------
#	elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if(ypage == lcd_driver.lcd_refresh_ypage)
	{
		ypage%=GRAM_YPAGE_NUM;
#		if (LCD_COLOUR_BIT==1)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
#		elif (LCD_COLOUR_BIT==2)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] ^= u8_value;
#		elif (LCD_COLOUR_BIT==3)
	  lcd_driver.LCD_GRAM[ypage][x][0] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][1] ^= u8_value;
		lcd_driver.LCD_GRAM[ypage][x][2] ^= u8_value;
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	}
#	endif
}



/*--------------------------------------------------------------
  * 名称: Lcd_Set_Driver_Mode(lcd_driver_mode_t mode)
  * 传入: mode 驱动模式 设置笔刷(颜色)
  * 功能: 设置驱动方式
----------------------------------------------------------------*/
void Lcd_Set_Driver_Mode(lcd_driver_mode_t mode)
{
#		if (LCD_COLOUR_BIT==1)
	  switch(mode)
		{
			case write_0 :						//(普通)全屏写0显示
				lcd_driver.Write_GRAM = lcd_driver_Write_0;
				lcd_driver.Clear_GRAM = lcd_driver_Write_1;
				break;
			case write_1 :						//(普通)全屏写1显示
				lcd_driver.Write_GRAM = lcd_driver_Write_1;
				lcd_driver.Clear_GRAM = lcd_driver_Write_0;
				break;
			case write_inverse :			//(普通)全屏反转写入
				lcd_driver.Write_GRAM = lcd_driver_Write_inv;
				lcd_driver.Clear_GRAM = lcd_driver_Write_inv;
				break;
			case write_0_inBox:				//(高级)限制区域内写0
				lcd_driver.Write_GRAM = lcd_driver_Write_0_inBox;
				lcd_driver.Clear_GRAM = lcd_driver_Write_1_inBox;
				break;
			case write_1_inBox:				//(高级)限制区域内写1
				lcd_driver.Write_GRAM = lcd_driver_Write_1_inBox;
				lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;
				break;							
			case write_inverse_inBox:	//(高级)限制区域内反转
				lcd_driver.Write_GRAM = lcd_driver_Write_inv_inBox;
				lcd_driver.Clear_GRAM = lcd_driver_Write_inv_inBox;
				break;
		}
#		elif (LCD_COLOUR_BIT==2)
	  switch(mode)
		{
			case write_0 :						//(普通)全屏写0显示
				lcd_driver.Write_GRAM = lcd_driver_Write_0;
				lcd_driver.Clear_GRAM = lcd_driver_Write_3;
				break;
			case write_1 :						//(普通)全屏写1显示
				lcd_driver.Write_GRAM = lcd_driver_Write_1;
				lcd_driver.Clear_GRAM = lcd_driver_Write_0;
				break;
			case write_2 :						//(普通)全屏写2显示
				lcd_driver.Write_GRAM = lcd_driver_Write_2;
				lcd_driver.Clear_GRAM = lcd_driver_Write_0;
				break;
			case write_3 :						//(普通)全屏写3显示
				lcd_driver.Write_GRAM = lcd_driver_Write_3;
				lcd_driver.Clear_GRAM = lcd_driver_Write_0;
				break;
			case write_inverse :			//(普通)全屏反转写入
				lcd_driver.Write_GRAM = lcd_driver_Write_inv;
				lcd_driver.Clear_GRAM = lcd_driver_Write_inv;
				break;
			case write_0_inBox:				//(高级)限制区域内写0
				lcd_driver.Write_GRAM = lcd_driver_Write_0_inBox;
				lcd_driver.Clear_GRAM = lcd_driver_Write_1_inBox;
				break;
			case write_1_inBox:				//(高级)限制区域内写1
				lcd_driver.Write_GRAM = lcd_driver_Write_1_inBox;
				lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;
				break;		
			case write_2_inBox:				//(高级)限制区域内写1
				lcd_driver.Write_GRAM = lcd_driver_Write_2_inBox;
				lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;
				break;	
			case write_3_inBox:				//(高级)限制区域内写1
				lcd_driver.Write_GRAM = lcd_driver_Write_3_inBox;
				lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;
				break;	
			case write_inverse_inBox:	//(高级)限制区域内反转
				lcd_driver.Write_GRAM = lcd_driver_Write_inv_inBox;
				lcd_driver.Clear_GRAM = lcd_driver_Write_inv_inBox;
				break;
		}
#		elif (LCD_COLOUR_BIT==3)
		lcd_driver.Clear_GRAM = lcd_driver_Write_0;
	  switch(mode)
		{
			case write_0 :lcd_driver.Write_GRAM = lcd_driver_Write_0;lcd_driver.Clear_GRAM = lcd_driver_Write_0;break;
			case write_1 :lcd_driver.Write_GRAM = lcd_driver_Write_1;lcd_driver.Clear_GRAM = lcd_driver_Write_0;break;
			case write_2 :lcd_driver.Write_GRAM = lcd_driver_Write_2;lcd_driver.Clear_GRAM = lcd_driver_Write_0;break;
			case write_3 :lcd_driver.Write_GRAM = lcd_driver_Write_3;lcd_driver.Clear_GRAM = lcd_driver_Write_0;break;
			case write_4 :lcd_driver.Write_GRAM = lcd_driver_Write_4;lcd_driver.Clear_GRAM = lcd_driver_Write_0;break;
			case write_5 :lcd_driver.Write_GRAM = lcd_driver_Write_5;lcd_driver.Clear_GRAM = lcd_driver_Write_0;break;
			case write_6 :lcd_driver.Write_GRAM = lcd_driver_Write_6;lcd_driver.Clear_GRAM = lcd_driver_Write_0;break;
			case write_7 :lcd_driver.Write_GRAM = lcd_driver_Write_7;lcd_driver.Clear_GRAM = lcd_driver_Write_0;break;
			case write_inverse:lcd_driver.Write_GRAM = lcd_driver_Write_inv;lcd_driver.Clear_GRAM = lcd_driver_Write_inv;break;
			case write_0_inBox:lcd_driver.Write_GRAM = lcd_driver_Write_0_inBox;lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;break;
			case write_1_inBox:lcd_driver.Write_GRAM = lcd_driver_Write_1_inBox;lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;break;
			case write_2_inBox:lcd_driver.Write_GRAM = lcd_driver_Write_2_inBox;lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;break;
			case write_3_inBox:lcd_driver.Write_GRAM = lcd_driver_Write_3_inBox;lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;break;
			case write_4_inBox:lcd_driver.Write_GRAM = lcd_driver_Write_4_inBox;lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;break;
			case write_5_inBox:lcd_driver.Write_GRAM = lcd_driver_Write_5_inBox;lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;break;
			case write_6_inBox:lcd_driver.Write_GRAM = lcd_driver_Write_6_inBox;lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;break;
			case write_7_inBox:lcd_driver.Write_GRAM = lcd_driver_Write_7_inBox;lcd_driver.Clear_GRAM = lcd_driver_Write_0_inBox;break;
			case write_inverse_inBox:lcd_driver.Write_GRAM = lcd_driver_Write_inv_inBox;lcd_driver.Clear_GRAM = lcd_driver_Write_inv_inBox;break;
		}
#		else
		#error("not support this LCD_COLOUR_BIT!")
#		endif
	
}

//--------------------------------------------------------------驱动接口--------------------------------------------------------------


/*--------------------------------------------------------------
  * 名称: Lcd_Set_Driver_Box(uint8_t x_min ,uint8_t y_min ,uint8_t x_max,uint8_t y_max)
  * 传入: (x_min,y_min)起始点 (x_max,y_max)终点
  * 功能: 设置高级驱动的限制区域(Box)
----------------------------------------------------------------*/
void Lcd_Set_Driver_Box(uint16_t x_min ,uint16_t y_min ,uint16_t x_max,uint16_t y_max)
{
	lcd_driver.Box.x_min = x_min;
	lcd_driver.Box.x_max = x_max;
	lcd_driver.Box.ypage_min = y_min/8;
	lcd_driver.Box.ypage_max = y_max/8;
	
	y_min =  8 - y_min%8;
	lcd_driver.Box.ypage_min_temp = 0;
	while (y_min--)
	{
		lcd_driver.Box.ypage_min_temp = lcd_driver.Box.ypage_min_temp >>1;
		lcd_driver.Box.ypage_min_temp |= 0x80;
	}
	y_max = y_max%8;
	lcd_driver.Box.ypage_max_temp = 0;
	
	while (y_max--)
	{
		lcd_driver.Box.ypage_max_temp = lcd_driver.Box.ypage_max_temp <<1;
		lcd_driver.Box.ypage_max_temp |= 0x01;
	}
}


/*--------------------------------------------------------------
  * 名称: Lcd_Draw_One_Byte(int16_t x,int16_t y,uint8_t u8_value)
  * 传入1: (x,y)坐标
  * 传入2: u8_page 一字节点阵数据
  * 功能: 将u8_page值以对其坐标的方式写到显存
  * 说明: 坐标点x,y支持负数
----------------------------------------------------------------*/
static void Lcd_Draw_One_Byte(int16_t x,volatile int16_t y,uint8_t u8_page)
{
	//---------全屏缓存-----------
	#if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
	if((x<0)||(x>(SCREEN_WIDTH-1))||(y>=SCREEN_HIGH)||(y+8<0))
	{
		return;
	}
	//---------单页缓存-----------
	#elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((x<0)||(x>(SCREEN_WIDTH-1))||(y >= ((lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM)*8))||(y+8 < (lcd_driver.lcd_refresh_ypage*8)))
	{
		return;
	}
	#endif
	
	if (y>=0)
	{
		uint8_t start_page  = y/8;
		uint8_t page_offset = y%8;
		lcd_driver.Write_GRAM(x,start_page,(u8_page<<page_offset));
		if(page_offset!=0)
		{
			if(start_page < (((SCREEN_HIGH+7)/8)-1))
			{
				lcd_driver.Write_GRAM(x,start_page+1,(u8_page>>(8-page_offset)));
			}
		}
	}
	else if(y> -8)
	{
		lcd_driver.Write_GRAM(x,0,(u8_page>>(-y)));
	}
}




/*--------------------------------------------------------------
  * 名称: Lcd_Draw_Pixl(int16_t x,int16_t y)
  * 传入: x,y 坐标点
  * 功能: 绘制一个像素点
  * 说明: 支持负数不报错
----------------------------------------------------------------*/
void Lcd_Draw_Pixl(int16_t x,int16_t y)
{
	Lcd_Draw_One_Byte(x,y,0x01);
}




//高效率画垂直线, 输入x点 Y起点,Y终点
static void Lcd_Draw_VLine(int16_t x,int16_t y_min, int16_t y_max)
{
	uint8_t offset;
	uint8_t ypage;
	uint8_t i;
	
	if((x<0)||(x>=SCREEN_WIDTH))
	{
		return;
	}
	
	if(y_max < y_min)
	{
		int16_t temp = y_min;
		y_min = y_max;
		y_max = temp;
	}
	
	if((y_max < 0) ||(y_min > SCREEN_HIGH))
	{
		return;
	}

	if(y_min < 0)
	{
		y_min = 0;
	}
	if(y_max > (SCREEN_HIGH-1))
	{
		y_max = SCREEN_HIGH-1;
	}
	
	ypage   = y_min / 8;
	offset = y_min % 8;
	//处理第一ypage
	if(offset!=0)
	{
		i=0x00;
		while (offset<8)
		{
			if(y_min <= y_max)
			{
				i |= (1<<offset);
				y_min ++;
				offset ++;
			}
			else
			{
				lcd_driver.Write_GRAM(x,ypage,i);
				//LCD_GRAM[ypage][x] |= i;
				return;
			}
		}
		lcd_driver.Write_GRAM(x,ypage,i);
		//LCD_GRAM[ypage][x] |= i;
		ypage++;
	}
	y_min --;
	while(y_min <= y_max)
	{
		offset = y_max - y_min;
		//处理中间ypage
		if(offset>=8)
		{
			lcd_driver.Write_GRAM(x,ypage,0xff);
			//LCD_GRAM[ypage][x] |= 0xff;
			ypage++;
			y_min+=8;
		}
		//处理结束ypage
		else
		{
			lcd_driver.Write_GRAM(x,ypage,cal_1[offset]);
			//LCD_GRAM[ypage][x] |= cal_1[offset];
			break;
		}
	}
}

//高效率画水平线, 输入x点 Y起点,Y终点
static void Lcd_Draw_HLine(int16_t x_min ,int16_t y, int16_t x_max)
{
	uint8_t offset;
	uint8_t ypage;
	
	if((y<0)||(y>=SCREEN_HIGH))
	{
		return;
	}
	
	if(x_max < x_min)
	{
		int16_t temp = x_min;
		x_min = x_max;
		x_max = temp;
	}
	
	if((x_max < 0) ||(x_min > SCREEN_WIDTH))
	{
		return;
	}

	if(x_min < 0)
	{
		x_min = 0;
	}
	if(x_max > (SCREEN_WIDTH-1))
	{
		x_max = SCREEN_WIDTH-1;
	}
	
	ypage   = y / 8;
	offset = 0x01 << (y % 8);
	
	while(x_min<=x_max)
	{
		lcd_driver.Write_GRAM(x_min,ypage,offset);
		x_min++;
	}
}


/*--------------------------------------------------------------
  * 名称: Lcd_Draw_Line(int16_t x1,int16_t y1,int16_t x2,int16_t y2)
  * 传入: (x1,y1)起点 (x0,y0)终点
  * 功能: 两点绘制一条直线
  * 说明: 布雷森汉姆直线算法
----------------------------------------------------------------*/
void Lcd_Draw_Line(int16_t x1,int16_t y1,int16_t x2,int16_t y2)
{
	uint16_t t; 
	int16_t xerr=0,yerr=0,delta_x,delta_y,distance;
	int16_t incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)//垂直线 
	{
		Lcd_Draw_VLine(x1,y1,y2);//高效率绘制垂直线
		return;
	}
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)//水平线 
	{
		Lcd_Draw_HLine(x1,y1,x2);//高效率绘制水平线
		return;
	}
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		Lcd_Draw_Pixl(uRow,uCol);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
	Lcd_Draw_Pixl(x2,y2);//画点
}


/*--------------------------------------------------------------
  * Lcd_Draw_Circel_part(int16_t x0,int16_t y0,uint16_t r,circle_part_t cPart)
  * 传入: (x0,y0):起点  r:半径 cPart:圆的部分
  * 功能: 绘制圆形部分
----------------------------------------------------------------*/
void Lcd_Draw_Circel_part(int16_t x0,int16_t y0,uint16_t r,circle_part_t cPart)
{
	int16_t x = 0;
	int16_t y = r;
	int16_t d = 3 - r*2;
	
	while(x <= y)
	{
		
		if(cPart&C_RU)Lcd_Draw_Pixl(x0 + y, y0 - x);
		if(cPart&C_UR)Lcd_Draw_Pixl(x0 + x, y0 - y);
		if(cPart&C_UL)Lcd_Draw_Pixl(x0 - x, y0 - y);
		if(cPart&C_LU)Lcd_Draw_Pixl(x0 - y, y0 - x);
		if(cPart&C_LD)Lcd_Draw_Pixl(x0 - y, y0 + x);
		if(cPart&C_DL)Lcd_Draw_Pixl(x0 - x, y0 + y);
		if(cPart&C_DR)Lcd_Draw_Pixl(x0 + x, y0 + y);
		if(cPart&C_RD)Lcd_Draw_Pixl(x0 + y, y0 + x);
		
		
		if(d < 0)
		{
			d += x*4 + 6;
		}
		else
		{
			d += 10 + (x-y)*4;
			y--;
		}
		x++;
	}
	
	//---专门处理反色驱动---
	x--;
	if((lcd_driver.Write_GRAM == lcd_driver_Write_inv)||
		(lcd_driver.Write_GRAM == lcd_driver_Write_1_inBox))
	{

		if((cPart&(C_RD|C_DR))==(C_RD|C_DR))Lcd_Draw_Pixl(x0+x,y0+x);//补右下角边边界重叠点
		if((cPart&(C_LU|C_UL))==(C_LU|C_UL))Lcd_Draw_Pixl(x0-x,y0-x);//补左上角边边界重叠点
		if((cPart&(C_LD|C_DL))==(C_LD|C_DL))Lcd_Draw_Pixl(x0-x,y0+x);//补左下角边边界重叠点
		if((cPart&(C_RU|C_UR))==(C_RU|C_UR))Lcd_Draw_Pixl(x0+x,y0-x);//补右上角边边界重叠点
		if((cPart&(C_RU|C_RD))==(C_RU|C_RD))Lcd_Draw_Pixl(x0+r,y0);//补右边边界重叠点
		if((cPart&(C_LU|C_LD))==(C_LU|C_LD))Lcd_Draw_Pixl(x0-r,y0);//补左边边界重叠点
		if((cPart&(C_UL|C_UR))==(C_UL|C_UR))Lcd_Draw_Pixl(x0,y0-r);//补上边边界重叠点
		if((cPart&(C_DL|C_DR))==(C_DL|C_DR))Lcd_Draw_Pixl(x0,y0+r);//补下边边界重叠点
	}
}

/*--------------------------------------------------------------
  * Lcd_Fill_Circel_part(int16_t x0,int16_t y0,uint16_t r,circle_part_t cPart)
  * 传入: (x0,y0):起点  r:半径 cPart:圆的部分
  * 功能: 填充圆形部分
----------------------------------------------------------------*/
void Lcd_Fill_Circel_part(int16_t x0,int16_t y0,uint16_t r,circle_part_t cPart)
{
	int16_t x = 0;
	int16_t y = r;
	int16_t d = 3 - r*2;
	uint16_t j;
	
	while(x <= y)
	{
			//填充圆角
			if(cPart&C_RU)Lcd_Draw_HLine(x0 + y, y0 - x, x0+x);//Lcd_Draw_Line(x0 + y, y0 - x, x0 + y, y0);
			if(cPart&C_UR)Lcd_Draw_VLine(x0 + x, y0 - y, y0-x);//Lcd_Draw_Line(x0 + x, y0 - y, x0, y0 - y);
			if(cPart&C_UL)Lcd_Draw_VLine(x0 - x, y0 - y, y0-x);//Lcd_Draw_Line(x0 - x, y0 - y, x0, y0 - y);
			if(cPart&C_LU)Lcd_Draw_HLine(x0 - y, y0 - x, x0-x);//Lcd_Draw_Line(x0 - y, y0 - x, x0 - y, y0);
			if(cPart&C_LD)Lcd_Draw_HLine(x0 - y, y0 + x, x0-x);//Lcd_Draw_Line(x0 - y, y0 + x, x0 - y, y0);
			if(cPart&C_DL)Lcd_Draw_VLine(x0 - x, y0 + y, y0+x);//Lcd_Draw_Line(x0 - x, y0 + y, x0, y0 + y);
			if(cPart&C_DR)Lcd_Draw_VLine(x0 + x, y0 + y, y0+x);//Lcd_Draw_Line(x0 + x, y0 + y, x0, y0 + y);
			if(cPart&C_RD)Lcd_Draw_HLine(x0 + y, y0 + x, x0+x);//Lcd_Draw_Line(x0 + y, y0 + x, x0 + y, y0);
		if(d < 0)
		{
			d += x*4 + 6;
		}
		else
		{
			d += 10 + (x-y)*4;
			y--;
		}
		x++;
	}
	
	
	
	//---专门处理反色驱动---
	if((lcd_driver.Write_GRAM == lcd_driver_Write_inv)||
		(lcd_driver.Write_GRAM == lcd_driver_Write_1_inBox))
	{
		j=0;
		while(j<x)
		{
			if((cPart&(C_RD|C_DR))==(C_RD|C_DR))Lcd_Draw_Pixl(x0+j,y0+j);//画点
			if((cPart&(C_LU|C_UL))==(C_LU|C_UL))Lcd_Draw_Pixl(x0-j,y0-j);//画点
			if((cPart&(C_LD|C_DL))==(C_LD|C_DL))Lcd_Draw_Pixl(x0-j,y0+j);//画点
			if((cPart&(C_RU|C_UR))==(C_RU|C_UR))Lcd_Draw_Pixl(x0+j,y0-j);//画点
			j++;
		}
		if((cPart&(C_RU|C_RD))==(C_RU|C_RD))Lcd_Draw_HLine(x0, y0, x0+r);
		if((cPart&(C_LU|C_LD))==(C_LU|C_LD))Lcd_Draw_HLine(x0-r, y0, x0);
		if((cPart&(C_UL|C_UR))==(C_UL|C_UR))Lcd_Draw_VLine(x0, y0, y0+r);
		if((cPart&(C_DL|C_DR))==(C_DL|C_DR))Lcd_Draw_VLine(x0, y0-r, y0);
	}
	
}




/*--------------------------------------------------------------
  * 名称: Lcd_Fill_Box(int16_t x_min,int16_t y_min, int16_t x_max, int16_t y_max)
  * 传入: 4点坐标
  * 功能: 填充矩形
  * 说明: 坐标点支持负数
----------------------------------------------------------------*/
void Lcd_Fill_Box(int16_t x_min,int16_t y_min, int16_t x_max, int16_t y_max)
{
	uint8_t ypage;
	uint8_t offset;
	uint16_t x;

	if(x_max < x_min)
	{
		int16_t temp = x_min;
		x_min = x_max;
		x_max = temp;
	}
	if(y_max < y_min)
	{
		int16_t temp = y_min;
		y_min = y_max;
		y_max = temp;
	}
	if(x_min < 0){x_min=0;}
	if(y_min < 0){y_min=0;}
	if(x_max > (SCREEN_WIDTH-1)){x_max = (SCREEN_WIDTH-1);}

	
	//---------全屏缓存-----------
	#if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
	if(y_max > (SCREEN_HIGH-1)){y_max = (SCREEN_HIGH-1);}
	//---------单页缓存-----------
	#elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	uint16_t max = (lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM)*8;
	if(y_max > max){y_max = max;}
	if((y_min >= max)||(y_max < (lcd_driver.lcd_refresh_ypage*8)))
	{
		return;
	}
	#endif
	if((x_min>x_max)||(y_min>y_max)){return;}//错误值,不绘画
	

	
	ypage   = y_min / 8;
	offset = y_min % 8;
	x = x_min;
	//处理第一ypage
	if(offset!=0)
	{
		uint8_t i=0x00;
		while (offset<8)
		{
			if(y_min <= y_max)
			{
				i |= (1<<offset);
				y_min ++;
				offset ++;
			}
			else
			{
				while(x <= x_max)
				{
					lcd_driver.Write_GRAM(x,ypage,i);
					//LCD_GRAM[ypage][x] |= i;
					x++;
				}
				return;
			}
		}
		while(x <= x_max)
		{
			lcd_driver.Write_GRAM(x,ypage,i);
			//LCD_GRAM[ypage][x] |= i;
			x++;
		}
		x = x_min;
		ypage++;
	}
	y_min --;
	while(y_min <= y_max)
	{
		offset = y_max - y_min;
		//处理中间ypage
		if(offset>=8)
		{
			while(x <= x_max)
			{
				lcd_driver.Write_GRAM(x,ypage,0xff);
				//LCD_GRAM[ypage][x] |= 0xff;
				x++;
			}
			x = x_min;
			ypage++;
			y_min+=8;
		}
		//处理结束ypage
		else
		{
			while(x <= x_max)
			{
				lcd_driver.Write_GRAM(x,ypage,cal_1[offset]);
				//LCD_GRAM[ypage][x] |= cal_1[offset];
				x++;
			}
			break;
		}
	}
}





/*--------------------------------------------------------------
  * 名称: Lcd_Draw_Box(int16_t x_min,int16_t y_min, int16_t x_max, int16_t y_max)
  * 传入: (x_min,y_min)起点 (x_max,y_max)终点
  * 功能: 绘制矩形
----------------------------------------------------------------*/
void Lcd_Draw_Box(int16_t x_min,int16_t y_min, int16_t x_max, int16_t y_max)
{
	Lcd_Draw_VLine(x_min,y_min,y_max);
	Lcd_Draw_VLine(x_max,y_min,y_max);
	Lcd_Draw_HLine(x_min,y_min,x_max);
	Lcd_Draw_HLine(x_min,y_max,x_max);
}

/*--------------------------------------------------------------
  * 名称: Lcd_Draw_Bitmap(int16_t x0,int16_t y0,uint16_t sizex,uint8_t sizey,uint8_t BMP[])
  * 传入1: x0 坐标左上角横坐标点
	* 传入2: y0 坐标左上角纵坐标点
  * 传入3: sizex 点阵图形x宽度
  * 传入4: sizey 点阵图形y高度
  * 传入5: BMP[] 点阵图形数组地址
  * 功能: 将点阵图形摆放到任意坐标点上
  * 说明: 坐标点支持负数
----------------------------------------------------------------*/
void Lcd_Draw_Bitmap(int16_t x0,int16_t y0,uint16_t sizex,uint16_t sizey,uint8_t BMP[])
{
	uint32_t i;
	uint16_t xi;
	uint16_t yi;
	
	//---------全屏缓存-----------
	#if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存

	//---------单页缓存-----------
	#elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((x0+sizex<0)||(x0>(SCREEN_WIDTH-1))||(y0 >= ((lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM)*8))||(y0+sizey < (lcd_driver.lcd_refresh_ypage*8)))
	{
		return;
	}
	#endif
	
	i=0;
	for(yi=0;yi<sizey;yi+=8)
	{
		int16_t y=y0+yi;
		if(y>=SCREEN_HIGH)
		{
			break;
		}
		for(xi=0;xi<sizex;xi++)
		{
			int16_t x=x0+xi;
			if(x<SCREEN_WIDTH)
			{
				Lcd_Draw_One_Byte(x,y,BMP[i]);
			}
			i++;
		}
	}
}









/*--------------------------------------------------------------
  * 名称: Lcd_Fill_RBox(int16_t x_min,int16_t y_min, int16_t x_max, int16_t y_max, int8_t r)
  * 传入: (x_min,y_min)起点 (x_max,y_max)终点 r:半径
  * 功能: 填充倒圆角矩形
----------------------------------------------------------------*/
void Lcd_Fill_RBox(int16_t x_min,int16_t y_min, int16_t x_max, int16_t y_max, int8_t r)
{
	int16_t x_min_add_r;
	int16_t x_max_dec_r;
	int16_t y_min_add_r;
	int16_t y_max_dec_r;
	
	if((x_min>=x_max)||(y_min>=y_max)){return;}//错误值,不绘画
	if(r<0){r=0;}
	
	while((x_max - x_min)<(r*2)){r--;}
	while((y_max - y_min)<(r*2)){r--;}
	
	x_min_add_r = x_min + r;
	x_max_dec_r = x_max - r;
	y_min_add_r = y_min + r;
	y_max_dec_r = y_max - r;
	
	Lcd_Fill_Circel_part(x_min_add_r,y_min_add_r,r,C_QLU);//左上
	Lcd_Fill_Circel_part(x_max_dec_r,y_min_add_r,r,C_QRU);//右上
	Lcd_Fill_Circel_part(x_min_add_r,y_max_dec_r,r,C_QLD);//左下
	Lcd_Fill_Circel_part(x_max_dec_r,y_max_dec_r,r,C_QRD);//右下
							
		//左长条
	Lcd_Fill_Box	(x_min,
								y_min_add_r,
								x_min_add_r,
								y_max_dec_r);
	//中间长条			
	Lcd_Fill_Box	(x_min_add_r,
								y_min,
								x_max_dec_r,
								y_max);
	//右长条					
	Lcd_Fill_Box	(x_max_dec_r,
								y_min_add_r,
								x_max,
								y_max_dec_r);
								
				
	//---专门处理用于反色驱动---
	if((lcd_driver.Write_GRAM == lcd_driver_Write_inv)||
		(lcd_driver.Write_GRAM == lcd_driver_Write_1_inBox))
	{
		Lcd_Draw_HLine(x_min,y_min_add_r,x_min_add_r);
		Lcd_Draw_HLine(x_max_dec_r,y_min_add_r,x_max);
		Lcd_Draw_HLine(x_min,y_max_dec_r,x_min_add_r);
		Lcd_Draw_HLine(x_max_dec_r,y_max_dec_r,x_max);
		Lcd_Draw_VLine(x_min_add_r,y_min,y_max);
		Lcd_Draw_VLine(x_max_dec_r,y_min,y_max);
	}
}




/*--------------------------------------------------------------
  * 名称: Lcd_Draw_RBox(int16_t x_min,int16_t y_min, int16_t x_max, int16_t y_max, uint16_t r)
  * 传入: (x_min,y_min)起点 (x_max,y_max)终点 r:半径
  * 功能: 绘制倒圆角镂空矩形
----------------------------------------------------------------*/
void Lcd_Draw_RBox(int16_t x_min,int16_t y_min, int16_t x_max, int16_t y_max, uint16_t r)
{
	int16_t x_min_add_r;
	int16_t x_max_dec_r;
	int16_t y_min_add_r;
	int16_t y_max_dec_r;
	
	if(x_max < x_min)
	{
		int8_t temp = x_min;
		x_min = x_max;
		x_max = temp;
	}
	if(y_max < y_min)
	{
		int8_t temp = y_min;
		y_min = y_max;
		y_max = temp;
	}
	
	//if((x_min>=x_max)||(y_min>=y_max)){return;}//错误值,不绘画
	
	
	while((x_max - x_min)<(r*2)){r--;}
	while((y_max - y_min)<(r*2)){r--;}
	//if(r<0){r=0;}
	
	x_min_add_r = x_min + r;
	x_max_dec_r = x_max - r;
	y_min_add_r = y_min + r;
	y_max_dec_r = y_max - r;
	
	Lcd_Draw_Circel_part(x_min_add_r,y_min_add_r,r,C_QLU);//左上
	Lcd_Draw_Circel_part(x_max_dec_r,y_min_add_r,r,C_QRU);//右上
	Lcd_Draw_Circel_part(x_min_add_r,y_max_dec_r,r,C_QLD);//左下
	Lcd_Draw_Circel_part(x_max_dec_r,y_max_dec_r,r,C_QRD);//右下
	
	Lcd_Draw_VLine(x_min,y_min_add_r,y_max_dec_r);
	Lcd_Draw_VLine(x_max,y_min_add_r,y_max_dec_r);
	Lcd_Draw_HLine(x_min_add_r,y_min,x_max_dec_r);
	Lcd_Draw_HLine(x_min_add_r,y_max,x_max_dec_r);
	
	//---专门处理用于反色驱动---
	if((lcd_driver.Write_GRAM == lcd_driver_Write_inv)||
		(lcd_driver.Write_GRAM == lcd_driver_Write_1_inBox))
	{
		Lcd_Draw_Pixl(x_min,y_min_add_r);
		Lcd_Draw_Pixl(x_min,y_max_dec_r);
		Lcd_Draw_Pixl(x_min_add_r,y_min);
		Lcd_Draw_Pixl(x_min_add_r,y_max);
		Lcd_Draw_Pixl(x_max_dec_r,y_min);
		Lcd_Draw_Pixl(x_max_dec_r,y_max);
		Lcd_Draw_Pixl(x_max,y_min_add_r);
		Lcd_Draw_Pixl(x_max,y_max_dec_r);
	}
	
}

/*--------------------------------------------------------------
  * 名称: Lcd_Draw_Ascii(int16_t x,int16_t y,uint8_t chr)
  * 传入: (x,y)左上角坐标 char:字符
  * 功能: 绘制一个Ascii字符
----------------------------------------------------------------*/
void Lcd_Draw_Ascii(int16_t x,int16_t y,uint8_t chr)
{

	//---------全屏缓存-----------
	#if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
	
	//---------单页缓存-----------
	#elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((y > (lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM)*8)|| ((y + lcd_driver.fonts_ASCII->high) < (lcd_driver.lcd_refresh_ypage*8)))
	{
		return;
	}
	#endif
	
	
		//不存在的值
	if((chr<0x20)||(chr>=0x7F))
	{
		return;
	}
	
	Lcd_Draw_Bitmap(
	x,//左上角坐标x
	y,//左上角坐标y
	lcd_driver.fonts_ASCII->width,//字体宽度
	lcd_driver.fonts_ASCII->high ,//字体高度
	(uint8_t*)(lcd_driver.fonts_ASCII->font + lcd_driver.fonts_ASCII->byte_size * (chr-0x20)));
}

/*--------------------------------------------------------------
  * 名称: void Lcd_Draw_int32(int16_t x,int16_t y,int16_t num)//写数字,自动长度,32位带符号
  * 传入: (x,y)左上角坐标 num带符号16位数字
  * 功能: 根据输入的num数字用对应的"ASCII字库"绘制到屏幕对应坐标上
----------------------------------------------------------------*/
void Lcd_Draw_int32(int16_t x,int16_t y,int16_t num)
{
	uint32_t sum=1;
	if(num < 0)
	{
		Lcd_Draw_Bitmap(x,y,lcd_driver.fonts_ASCII->width,lcd_driver.fonts_ASCII->high,(uint8_t*)(lcd_driver.fonts_ASCII->font + lcd_driver.fonts_ASCII->byte_size * ('-'-0x20)));
		x += lcd_driver.fonts_ASCII->width + lcd_driver.fonts_ASCII->scape;
		num = -num;
	}
	else if(num == 0)
	{
		Lcd_Draw_Bitmap(x,y,lcd_driver.fonts_ASCII->width,lcd_driver.fonts_ASCII->high,(uint8_t*)(lcd_driver.fonts_ASCII->font + lcd_driver.fonts_ASCII->byte_size * ('0'-0x20)));
		return;
	}
	else if(num == 1)
	{
		Lcd_Draw_Bitmap(x,y,lcd_driver.fonts_ASCII->width,lcd_driver.fonts_ASCII->high,(uint8_t*)(lcd_driver.fonts_ASCII->font + lcd_driver.fonts_ASCII->byte_size * ('1'-0x20)));
		return;
	}
	while(sum <= (uint16_t)num)
	{
		sum = sum*10;
	}
	while(sum>1)
	{
		sum=sum/10;
		Lcd_Draw_Bitmap(x,y,lcd_driver.fonts_ASCII->width,lcd_driver.fonts_ASCII->high,(uint8_t*)(lcd_driver.fonts_ASCII->font + lcd_driver.fonts_ASCII->byte_size * (num/sum + '0'-0x20)));
		x += lcd_driver.fonts_ASCII->width + lcd_driver.fonts_ASCII->scape;
		num = (uint16_t)num % sum;
	}
}



/*--------------------------------------------------------------
  * 名称: void Lcd_Draw_Unicode(int16_t x,int16_t y,unicode_t unicode_id)
  * 传入: (x,y)左上角坐标 unicode_id
  * 功能: 根据输入的unicode_id寻找对应的"裁剪字库"绘制到屏幕坐标上
----------------------------------------------------------------*/
void Lcd_Draw_Unicode(int16_t x,int16_t y,unicode_t unicode_id)
{
	uint16_t i=0;
	//---------全屏缓存-----------
	#if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
	
	//---------单页缓存-----------
	#elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((y > (lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM)*8)|| ((y + lcd_driver.fonts_UTF8_cut->high) < (lcd_driver.lcd_refresh_ypage*8)))
	{
		return;
	}
	#endif
	
	
	while((lcd_driver.fonts_UTF8_cut->unicode_index[i]!=0x00)/*&&(lcd_driver.fonts_UTF8_cut->unicode_index[i+1]!=0x00)*/)
	{
		//-----------------A方式大小端读取方式根据MCU储存方式选择-------------------
		//if(((uint8_t*)lcd_driver.fonts_UTF8_cut->unicode_index)[i] == unicode_id.u8[1])
		//{
		//	if(lcd_driver.fonts_UTF8_cut->unicode_index[i+1] == unicode_id.u8[0])
		//	break;
		//}
		
		//-----------------B方式大小端读取方式根据MCU储存方式选择-------------------
		if(((uint8_t*)lcd_driver.fonts_UTF8_cut->unicode_index)[i] == unicode_id.u8[0])
		{
			if(lcd_driver.fonts_UTF8_cut->unicode_index[i+1] == unicode_id.u8[1])
			break;
		}
		
		i+=2;
	}
	
	if((lcd_driver.fonts_UTF8_cut->unicode_index[i]!=0x00)/*&&(lcd_driver.fonts_UTF8_cut->unicode_index[i+1]!=0x00)*/)
	{
		Lcd_Draw_Bitmap(
		x,
		y,
		lcd_driver.fonts_UTF8_cut->width,
		lcd_driver.fonts_UTF8_cut->high,
		(uint8_t*)lcd_driver.fonts_UTF8_cut->font + lcd_driver.fonts_UTF8_cut->byte_size*(i>>1));
	}
}


/*--------------------------------------------------------------
  * 名称: void Lcd_Draw_UTF8_String(int16_t x,int16_t y,uint8_t *p)
  * 传入: (x,y)左上角坐标 *p字符串指针
  * 功能: 在指定坐标上按照系统设定字体格式绘制字符串
----------------------------------------------------------------*/
void Lcd_Draw_UTF8_String(int16_t x,int16_t y,uint8_t *p)
{
	volatile int16_t x_0;
	
	
	//---------全屏缓存-----------
	#if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE)))//全屏缓存
	if((x>SCREEN_WIDTH)||(y>(SCREEN_HIGH-1)))
	{
		return;
	}
	x_0 = x;
	while((*p!=0x00)&&(y<SCREEN_HIGH))
	//---------单页缓存-----------
	#elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
	if((x>SCREEN_WIDTH)||(y>(lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM)*8))
	{
		return;
	}
	x_0 = x;
	while((*p!=0x00)&&(y<(lcd_driver.lcd_refresh_ypage+GRAM_YPAGE_NUM)*8))
	#endif
	{
		uint8_t temp = *p & 0xF8;
		if(temp<=0x7F)//单字节
		//if(temp < 0xC0)	
		{
			//单字节均是ASCII
			if(*p == '\r')
			{
				x = x_0;
				y += lcd_driver.newline_high;
				p+=1;
				if(*p == '\n'){p+=1;}
			}
			else if(*p == '\n')
			{
				x = x_0;
				y += lcd_driver.newline_high;
				p+=1;
			}
			else
			{
				Lcd_Draw_Ascii(x,y,*p);
				x += lcd_driver.fonts_ASCII->width + lcd_driver.fonts_ASCII->scape;
				p+=1;
			}
		}
		else if(temp < 0xE0)//双字节
		{
			//双字节好像都是符号
			/*
			u16_u8_t unicode_id;
			unicode_id.u8[0]=(*p<<3)|((*(p+1)>>3)&0x07);
			unicode_id.u8[1]=(*(p+1)&0x07);
			Lcd_Draw_Unicode(x,y,unicode_id);
			x += Wegui_font.fonts_HZ_With + Wegui_font.fonts_HZ_scape;
			*/
			p+=2;
		}
		else if(temp < 0xF0)//三字节(都是中文)
		{
			//三字节都是中文
			unicode_t unicode_id;
			unicode_id.u8[0]=(*p<<4)|((*(p+1)>>2)&0x0F);
			unicode_id.u8[1]=(*(p+1)<<6)|((*(p+2))&0x3F);
			Lcd_Draw_Unicode(x,y,unicode_id);
			x += lcd_driver.fonts_UTF8_cut->width + lcd_driver.fonts_UTF8_cut->scape;
			p+=3;
		}
		else//四字节
		{
			p+=4;
		}
	}
}


/*--------------------------------------------------------------
  * 名称: Lcd_Get_UTF8_XLen(uint8_t *p)
  * 传入: *p字符串指针
  * 功能: 返回字符串在系统字体上x方向的总像素长度
----------------------------------------------------------------*/
uint16_t Lcd_Get_UTF8_XLen(uint8_t *p)
{
	uint16_t len=0;
	uint16_t temp_len=0;
	uint16_t endscape=0;
	while(1)
	{
		uint8_t temp = *p & 0xF8;
		if(*p==0x00)
		{
			if(temp_len>endscape)
			{
				temp_len -= endscape;
			}
			if(len < temp_len){len = temp_len;}
			temp_len = 0;
			break;
		}
		else if(*p == '\r')
		{
			p+=1;
			if(*p == '\n'){p+=1;}
			if(temp_len>endscape)
			{
				temp_len -= endscape;
			}
			if(len < temp_len){len = temp_len;}
			temp_len = 0;
		}
		else if(*p == '\n')
		{
			p+=1;
			if(temp_len>endscape)
			{
				temp_len -= endscape;
			}
			if(len < temp_len){len = temp_len;}
			temp_len = 0;
		}

		else if(temp<=0x7F)//单字节
		//if(temp < 0xC0)	
		{
			//单字节均是ASCII
			temp_len += lcd_driver.fonts_ASCII->width+lcd_driver.fonts_ASCII->scape;
			
			endscape=lcd_driver.fonts_ASCII->scape;
			p+=1;
		}
		else if(temp < 0xE0)//双字节
		{
			//双字节好像都是符号
			/*
			u16_u8_t unicode_id;
			unicode_id.u8[0]=(*p<<3)|((*(p+1)>>3)&0x07);
			unicode_id.u8[1]=(*(p+1)&0x07);
			Lcd_Draw_Unicode(x,y,unicode_id);
			x += Wegui_font.fonts_HZ_With + Wegui_font.fonts_HZ_scape;
			*/
			p+=2;
		}
		else if(temp < 0xF0)//三字节(都是中文)
		{
			//三字节都是中文
			temp_len += lcd_driver.fonts_UTF8_cut->width + lcd_driver.fonts_UTF8_cut->scape;
			endscape=lcd_driver.fonts_UTF8_cut->scape;
			p+=3;
		}
		else//四字节
		{
			p+=4;
		}
	}

	return len;
}


/*--------------------------------------------------------------
  * 名称: Lcd_Get_UTF8_YLine(int16_t x,int16_t y,uint8_t chr)
  * 传入: *p字符串指针
  * 功能: 返回UTF8字符串换行的次数
----------------------------------------------------------------*/
uint16_t Lcd_Get_UTF8_YLine(uint8_t *p)
{
	uint16_t line=1;
	while(1)
	{
		uint8_t temp = *p & 0xF8;
		if(*p==0x00)
		{
			break;
		}
		if(*p == '\r')
		{
			p+=1;
			if(*p == '\n'){p+=1;}
			line++;
		}
		else if(*p == '\n')
		{
			p+=1;
			line++;
		}
		else if(temp<=0x7F)//单字节
		//else if(temp < 0xC0)	
		{
			//单字节均是ASCII
			p+=1;
		}
		else if(temp < 0xE0)//双字节
		{
			p+=2;
		}
		else if(temp < 0xF0)//三字节(都是中文)
		{
			p+=3;
		}
		else//四字节
		{
			p+=4;
		}
	}
	return line;
}




/*--------------------------------------------------------------
  * 名称: Lcd_Clear_GRAM(void)
	* 功能: 清显存,全部清为0x00
----------------------------------------------------------------*/
void Lcd_Clear_GRAM(void)
{
	unsigned int i,j,k;
	for (k=0;k<LCD_COLOUR_BIT;k++)
	{
		for(i=0;i<GRAM_YPAGE_NUM;i++)
		{
				for(j=0;j<SCREEN_WIDTH;j++)
				{
					lcd_driver.LCD_GRAM[i][j][k]=0x00;
				}
		}
	}
}

/*--------------------------------------------------------------
  * 名称: Lcd_Fill_GRAM(uint8_t n)
	* 功能: 显存全部刷成"笔刷n"(lcd_driver.colour[n])的颜色
----------------------------------------------------------------*/
void Lcd_Fill_GRAM(uint8_t n)
{
	
	unsigned int i,j;
	uint8_t k;
	
	if(n&0x01){k=0xff;}
	else{k=0x00;}
	for(i=0;i<GRAM_YPAGE_NUM;i++)
	{
			for(j=0;j<SCREEN_WIDTH;j++)
			{
				lcd_driver.LCD_GRAM[i][j][0]=k;
			}
	}
	#if (LCD_COLOUR_BIT >= 2)
	if(n&0x02){k=0xff;}
	else{k=0x00;}
	for(i=0;i<GRAM_YPAGE_NUM;i++)
	{
			for(j=0;j<SCREEN_WIDTH;j++)
			{
				lcd_driver.LCD_GRAM[i][j][1]=k;
			}
	}
	#endif
	#if (LCD_COLOUR_BIT >= 3)
	if(n&0x04){k=0xff;}
	else{k=0x00;}
	for(i=0;i<GRAM_YPAGE_NUM;i++)
	{
			for(j=0;j<SCREEN_WIDTH;j++)
			{
				lcd_driver.LCD_GRAM[i][j][2]=k;
			}
	}
	#endif
}





/*--------------------------------------------------------------
  * 名称: lcd_driver_Init()
  * 功能: 驱动初始化
----------------------------------------------------------------*/
void lcd_driver_Init(void)
{
	//-------------初始化-------------
	LCD_Port_Init();
	LCD_IC_Init();
	
	
	//------driver配置默认字体---------
	//---中英文字体high高度建议一致----
	
	#if(SCREEN_WIDTH < 240)
		lcd_driver.fonts_ASCII = &ascii_SongTi_6X12;//默认ASCII字体
		lcd_driver.fonts_UTF8_cut = &SongTi_UTF8_12X12;//默认UTF8字体(裁切)
	#else
		lcd_driver.fonts_ASCII = &ascii_SongTi_8X16;//默认ASCII字体
		lcd_driver.fonts_UTF8_cut = &SongTi_UTF8_16X16;//默认UTF8字体(裁切)
	#endif

	//lcd_driver.newline_high = lcd_driver.fonts_ASCII->high;   //文本换行距离 (如果没有使用中文,可选择)
	lcd_driver.newline_high = lcd_driver.fonts_UTF8_cut->high;//文本换行距离 (选择ASCII字体和UTF8字体最大的一个)
	
	
#	ifdef LCD_USE_RGB565
	//设置默认笔刷颜色 RGB屏幕在程序运行过程中允许修改笔刷颜色
#		if (LCD_COLOUR_BIT>=1)
			RGB_Set_Driver_Colour(0,0x0000);//设置write_0驱动的笔刷颜色
			RGB_Set_Driver_Colour(1,0xFFFF);//设置write_1驱动的笔刷颜色
#		endif
#		if (LCD_COLOUR_BIT>=2)
			RGB_Set_Driver_Colour(2,0xFFFF);//设置write_2驱动的笔刷颜色
			RGB_Set_Driver_Colour(3,0xFFFF);//设置write_3驱动的笔刷颜色
#		endif
#		if (LCD_COLOUR_BIT>=3)
			RGB_Set_Driver_Colour(4,0xFFFF);//设置write_4驱动的笔刷颜色
			RGB_Set_Driver_Colour(5,0xFFFF);//设置write_5驱动的笔刷颜色
			RGB_Set_Driver_Colour(6,0xFFFF);//设置write_6驱动的笔刷颜色
			RGB_Set_Driver_Colour(7,0xFFFF);//设置write_7驱动的笔刷颜色
#		endif
#	endif
	
	//----------选择默认"笔刷"-----------
	//Lcd_Set_Driver_Mode(write_0);            //写0模式(普通) 单色时黑点模式 多色时笔刷颜色为lcd_driver.colour[0]
	Lcd_Set_Driver_Mode(write_1);              //写1模式(普通) 单色时白点模式 多色时笔刷颜色为lcd_driver.colour[1]
	//Lcd_Set_Driver_Mode(write_2);            //写2模式(普通) 多色时笔刷颜色为lcd_driver.colour[2]
	//Lcd_Set_Driver_Mode(write_3);            //写3模式(普通) 多色时笔刷颜色为lcd_driver.colour[3]
	//Lcd_Set_Driver_Mode(write_4);            //写4模式(普通) 多色时笔刷颜色为lcd_driver.colour[4]
	//Lcd_Set_Driver_Mode(write_5);            //写5模式(普通) 多色时笔刷颜色为lcd_driver.colour[5]
	//Lcd_Set_Driver_Mode(write_6);            //写6模式(普通) 多色时笔刷颜色为lcd_driver.colour[6]
	//Lcd_Set_Driver_Mode(write_7);            //写7模式(普通) 多色时笔刷颜色为lcd_driver.colour[7]
	//Lcd_Set_Driver_Mode(write_inverse);      //反写模式(普通) 单色时0->1或1->0 2位色时0(0B00)->3(0B11) 或1(0B01)->2(0B10)... 3位色时0(0B000)->7(0B111)或2(0B001)->6(0B110)...
	
	//Lcd_Set_Driver_Mode(write_0_inBox);      //限制区域内写0模式(高级) 单色时黑点模式 多色笔刷颜色为lcd_driver.colour[0]
	//Lcd_Set_Driver_Mode(write_1_inBox);      //限制区域内写1模式(高级) 单色时白点模式 多色笔刷颜色为lcd_driver.colour[1]
	//Lcd_Set_Driver_Mode(write_2_inBox);      //限制区域内写1模式(高级) 多色时笔刷颜色为lcd_driver.colour[2]
	//Lcd_Set_Driver_Mode(write_3_inBox);      //限制区域内写1模式(高级) 多色时笔刷颜色为lcd_driver.colour[3]
	//Lcd_Set_Driver_Mode(write_4_inBox);      //限制区域内写1模式(高级) 多色时笔刷颜色为lcd_driver.colour[4]
	//Lcd_Set_Driver_Mode(write_5_inBox);      //限制区域内写1模式(高级) 多色时笔刷颜色为lcd_driver.colour[5]
	//Lcd_Set_Driver_Mode(write_6_inBox);      //限制区域内写1模式(高级) 多色时笔刷颜色为lcd_driver.colour[6]
	//Lcd_Set_Driver_Mode(write_7_inBox);      //限制区域内写1模式(高级) 多色时笔刷颜色为lcd_driver.colour[7]
	//Lcd_Set_Driver_Mode(write_inverse_inBox);//限制区域内反写模式(高级) 单色时0->1或1->0 2位色时0(0B00)->3(0B11)或1(0B01)->2(0B10)... 3位色时0(0B000)->7(0B111)或2(0B001)->6(0B110)...
	
	Lcd_Set_Driver_Box(0,0,SCREEN_WIDTH-1,SCREEN_HIGH-1);//设置"高级"驱动的"限制区域"
	//----------默认驱动模式-----------
	
	//-----------显存初始化------------
	Lcd_Clear_GRAM();
	#if ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))//页缓存
		//页缓存才有lcd_driver.lcd_refresh_ypage
		lcd_driver.lcd_refresh_ypage=0;//页缓存专用变量, 记录当前刷的是第页
	#endif
	
	#if((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
		//动态刷新才有crc
		LCD_Reset_crc();//色彩已改变,强制刷新一下crc动态刷新值, 保证颜色正确
	#endif
	//-----------显存初始化------------
	
	while(LCD_Refresh()!=0);

}

	
