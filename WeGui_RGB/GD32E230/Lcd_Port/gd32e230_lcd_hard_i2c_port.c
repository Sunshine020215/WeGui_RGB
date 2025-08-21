/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#include "lcd_driver_config.h"
#include "lcd_wegui_config.h"

#if (LCD_PORT == _HARD_I2C)

#include "stdint.h"
#include "gd32e230_lcd_hard_i2c_port.h"
#include "lcd_driver.h"


#define LCD_RCU_I2Cx    RCU_I2C1
#define LCD_I2Cx        I2C1


#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
static uint16_t crc[((SCREEN_HIGH+7)/8)][PAGE_CRC_NUM];//储存crc
/*--------------------------------------------------------------
  * 名称: LCD_Reset_crc()
  * 功能: 刷新一次crc值(动态刷新专用)
  * 说明: 用于强制刷新屏幕,防止动态刷新出现区域不刷新
----------------------------------------------------------------*/
void LCD_Reset_crc()
{
	uint16_t i=0;
	uint16_t *p=&crc[0][0];
	while(i < ((SCREEN_HIGH+7)/8)*PAGE_CRC_NUM)
	{
		p[i]=0xff;//随机值,不是0x0就ok
		i++;
	}
}

//默认的动态刷新crc算法
#define lcd_crc() do \
{\
		CRC_CTL |= (uint32_t)CRC_CTL_RST;\
		uint8_t *i = &lcd_driver.LCD_GRAM[ypage][0][0];\
		for(x=0;x<(SCREEN_WIDTH*LCD_COLOUR_BIT);x++)\
		{\
			REG8(CRC) = i[x];\
		}\
		i_crc = CRC_DATA;\
}while(0)
#endif


void i2c_Init();
//出现错误复位i2c 防止卡死
#define i2c_err_check() do{timeout++;if((timeout>1000) || (0U != (REG32(LCD_I2Cx + I2C_STAT0_REG_OFFSET)&(uint32_t)0xDF00))){i2c_Init();return;}}while(0)

/*--------------------------------------------------------------
  * 名称: LCD_Send_1Cmd(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个命令
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Cmd(uint8_t dat)
{
	uint16_t timeout;
	//------------起始信号------------
	
	timeout=0;while(i2c_flag_get(LCD_I2Cx, I2C_FLAG_I2CBSY)){i2c_err_check();}
	i2c_start_on_bus(LCD_I2Cx);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_SBSEND)){i2c_err_check();}
	//------------发送地址------------	
	i2c_master_addressing(LCD_I2Cx, OLED_IIC_7ADDR<<1, I2C_TRANSMITTER);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_ADDSEND)){i2c_err_check();}
	i2c_flag_clear(LCD_I2Cx, I2C_FLAG_ADDSEND);
	//------------发送数据------------
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
	i2c_data_transmit(LCD_I2Cx, 0x00);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
	i2c_data_transmit(LCD_I2Cx, dat);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
	
	//------------结束信号------------                                                           
	i2c_stop_on_bus(LCD_I2Cx);
	timeout=0;while(I2C_CTL0(LCD_I2Cx) & I2C_CTL0_STOP){i2c_err_check();}
}

/*--------------------------------------------------------------
  * 名称: LCD_Send_1Dat(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个数据
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Dat(uint8_t dat)
{
	uint16_t timeout;
	//------------起始信号------------
	timeout=0;while(i2c_flag_get(LCD_I2Cx, I2C_FLAG_I2CBSY)){i2c_err_check();}
	i2c_start_on_bus(LCD_I2Cx);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_SBSEND)){i2c_err_check();}
	//------------发送地址------------	
	i2c_master_addressing(LCD_I2Cx, OLED_IIC_7ADDR<<1, I2C_TRANSMITTER);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_ADDSEND)){i2c_err_check();}
	i2c_flag_clear(LCD_I2Cx, I2C_FLAG_ADDSEND);
	//------------发送数据------------
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
	i2c_data_transmit(LCD_I2Cx, 0x40);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
	i2c_data_transmit(LCD_I2Cx, dat);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
	//------------结束信号------------                                                           
	i2c_stop_on_bus(LCD_I2Cx);
	while(I2C_CTL0(LCD_I2Cx) & I2C_CTL0_STOP){i2c_err_check();}
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
	uint16_t i;
	uint16_t timeout;
	//------------起始信号------------
	timeout=0;while(i2c_flag_get(LCD_I2Cx, I2C_FLAG_I2CBSY)){i2c_err_check();}
	i2c_start_on_bus(LCD_I2Cx);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_SBSEND)){i2c_err_check();}
	//------------发送地址------------	
	i2c_master_addressing(LCD_I2Cx, OLED_IIC_7ADDR<<1, I2C_TRANSMITTER);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_ADDSEND)){i2c_err_check();}
	i2c_flag_clear(LCD_I2Cx, I2C_FLAG_ADDSEND);
	//------------发送数据------------
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
	i2c_data_transmit(LCD_I2Cx, 0x40);
	i=0;
	while(num>i)
	{
		timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
		i2c_data_transmit(LCD_I2Cx, p[i]);
		i++;
	}       
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
	//------------结束信号------------                                                            
	i2c_stop_on_bus(LCD_I2Cx);
  timeout=0;while(I2C_CTL0(LCD_I2Cx) & I2C_CTL0_STOP){i2c_err_check();}                           
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
	uint16_t i;
	uint16_t timeout;
	//------------起始信号------------
	timeout=0;while(i2c_flag_get(LCD_I2Cx, I2C_FLAG_I2CBSY)){i2c_err_check();}
	i2c_start_on_bus(LCD_I2Cx);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_SBSEND)){i2c_err_check();}
	//------------发送地址------------	
	i2c_master_addressing(LCD_I2Cx, OLED_IIC_7ADDR<<1, I2C_TRANSMITTER);
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_ADDSEND)){i2c_err_check();}
	i2c_flag_clear(LCD_I2Cx, I2C_FLAG_ADDSEND);
	//------------发送数据------------
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
	i2c_data_transmit(LCD_I2Cx, 0x00);
	i=0;
	while(num>i)
	{
		timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
		i2c_data_transmit(LCD_I2Cx, p[i]);
		i++;
	}       
	timeout=0;while(!i2c_flag_get(LCD_I2Cx, I2C_FLAG_TBE)){i2c_err_check();}
	//------------结束信号------------                                                            
	i2c_stop_on_bus(LCD_I2Cx);
  timeout=0;while(I2C_CTL0(LCD_I2Cx) & I2C_CTL0_STOP){i2c_err_check();}                             
}



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

void i2c_Init()
{
	i2c_deinit(LCD_I2Cx);
	rcu_periph_clock_enable(LCD_RCU_I2Cx);
  i2c_clock_config(LCD_I2Cx, I2C_FREQ_INIT, I2C_DTCY_2);
  i2c_mode_addr_config(LCD_I2Cx, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, OLED_IIC_7ADDR);
  i2c_enable(LCD_I2Cx);
  i2c_ack_config(LCD_I2Cx, I2C_ACK_ENABLE);
}
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
	
	
	i2c_Init();
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
	rcu_periph_clock_enable(RCU_CRC);//动态刷新CRC校验用
	crc_polynomial_size_set(CRC_CTL_PS_16);//CRC位数
	crc_polynomial_set(0x04C11DB7);//CRC多项式值
	#endif
	
	LCD_RES_Clr();
	{
		volatile unsigned int i=10000;
		while(i--);
	}
	LCD_RES_Set();
	
}




/*--------------------------------------------------------------
  * 名称: LCD_Refresh(void)
  * 功能: 驱动接口,将显存LCD_GRAM全部内容发送至屏幕
----------------------------------------------------------------*/

//----------------------------普通OLED屏幕刷屏接口-------------------------------------
#if defined (LCD_USE_NORMAL_OLED)

#if (LCD_MODE == _FULL_BUFF_FULL_UPDATE)
//---------方式1:全缓存全屏刷新----------
uint8_t LCD_Refresh(void)
{
	uint8_t ypage;
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
	uint8_t ypage;
	uint16_t x;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint32_t i_crc;
		
		//-----方式1:CRC算法校验-----
		lcd_crc();//宏定义
		//---------------------------
		
		if(crc[ypage][0] != i_crc)
		{
			LCD_Set_Addr(0,ypage);
			LCD_Send_nDat(&lcd_driver.LCD_GRAM[ypage][0][0],SCREEN_WIDTH);
			crc[ypage][0] = i_crc;
		}
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
	uint8_t ypage;
	uint16_t x;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint32_t i_crc;
		//判断屏幕是否已刷完
		if((lcd_driver.lcd_refresh_ypage + ypage)>=((SCREEN_HIGH+7)/8))
		{
			break;
		}
		//-----方式1:CRC算法校验-----
		lcd_crc();//宏定义
		//---------------------------
		if(crc[lcd_driver.lcd_refresh_ypage + ypage][0] != i_crc)
		{
			LCD_Set_Addr(0,lcd_driver.lcd_refresh_ypage + ypage);
			LCD_Send_nDat(&lcd_driver.LCD_GRAM[ypage][0][0],SCREEN_WIDTH);
			crc[lcd_driver.lcd_refresh_ypage + ypage][0] = i_crc;
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
	#error("Gray OLED not support hard_iic driver yet! Use soft_iic please!")
	
	
//----------------------------RGB565屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_RGB565)
	//彩屏TFT屏不支持I2C方式驱动 请更改屏幕驱动IC型号
	#error ("TFT not support I2C Driver!");
#endif





#endif
