#include "lcd_driver_config.h"
#include "lcd_wegui_config.h"

#if(LCD_PORT == _HARD_4SPI)

#include "gd32e230_lcd_hard_4spi_port.h"
#include "lcd_driver.h"
#include "stdint.h"


#define PORT_SPIx       (0)
#define LCD_SPIx        SPI0
#define LCD_RCU_SPIx    RCU_SPI0


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

/*--------------------------------------------------------------
  * 名称: LCD_delay_ms(uint32_t ms)
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

/*--------------------------------------------------------------
  * 名称: LCD_Send_1Cmd(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个命令(DC=0时发1字节spi)
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Cmd(uint8_t dat)
{
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_DC_Clr();
	LCD_CS_Clr();
	{
		SPI_DATA(LCD_SPIx) = dat;//spi_i2s_data_transmit(LCD_SPIx, dat);
	}
	while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
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
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_DC_Set();
	LCD_CS_Clr();
	{
		SPI_DATA(LCD_SPIx) = dat;//spi_i2s_data_transmit(LCD_SPIx, dat);
	}
	while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
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
	uint16_t i=0;
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_DC_Set();
	LCD_CS_Clr();
	while(num>i)	  
	{
		while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
		SPI_DATA(LCD_SPIx) = p[i++];//spi_i2s_data_transmit(LCD_SPIx, p[i++]);
	}
	while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_CS_Set();
}



//-------------------------------------------OLED屏幕驱动接口----------------------------------------------
#if defined (LCD_USE_NORMAL_OLED) || defined (LCD_USE_GRAY_OLED)



/*--------------------------------------------------------------
  * 名称: LCD_Port_Init()
  * 传入: 无
  * 返回: 无
  * 功能: 屏幕接口初始化
  * 说明: 
----------------------------------------------------------------*/
void LCD_Port_Init(void)
{
	//OELD SPI SCL默认常高,第二个跳变数据沿有效
	spi_parameter_struct spi_init_struct=
	{
		.trans_mode           = SPI_TRANSMODE_FULLDUPLEX,
		.device_mode          = SPI_MASTER,
		.frame_size           = SPI_FRAMESIZE_8BIT,
		.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE,
		.nss                  = SPI_NSS_SOFT,
		.prescale             = SPI_PSC_x,
		.endian               = SPI_ENDIAN_MSB,
	};
	
	LCD_SCL_IO_Init();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	LCD_DC_IO_Init();
	LCD_CS_IO_Init();
	LCD_BL_IO_Init();
	
	spi_i2s_deinit(LCD_SPIx);
	
	rcu_periph_clock_enable(LCD_RCU_SPIx);
	spi_init(LCD_SPIx, &spi_init_struct);
	spi_enable(LCD_SPIx);
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
		rcu_periph_clock_enable(RCU_CRC);//动态刷新CRC校验用
		crc_polynomial_size_set(CRC_CTL_PS_32);//CRC位数
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
  * 名称: LCD_Send_nCmd(uint8_t *p,uint16_t num)
  * 传入1: *p数组指针
  * 传入2: num发送数量
  * 返回: 无
  * 功能: 向屏幕发送num个命令
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_nCmd(uint8_t *p,uint16_t num)
{
	uint16_t i=0;
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_DC_Clr();
	LCD_CS_Clr();
	while(num>i)	  
	{
		while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
		SPI_DATA(LCD_SPIx) = p[i++];//spi_i2s_data_transmit(LCD_SPIx, p[i++]);
	}
	while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_CS_Set();
}







//-------------------------------------------RGB屏幕驱动接口----------------------------------------------
#elif defined (LCD_USE_RGB565)
void LCD_Port_Init(void)
{
	//TFT SPI SCL默认常低,第一个跳变数据沿有效
	spi_parameter_struct spi_init_struct=
	{
		.trans_mode           = SPI_TRANSMODE_FULLDUPLEX,
		.device_mode          = SPI_MASTER,
		.frame_size           = SPI_FRAMESIZE_8BIT,
		.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE,
		.nss                  = SPI_NSS_SOFT,
		.prescale             = SPI_PSC_x,
		.endian               = SPI_ENDIAN_MSB,
	};
	
	LCD_SCL_IO_Init();
	LCD_SDA_IO_Init();
	LCD_RES_IO_Init();
	LCD_DC_IO_Init();
	LCD_CS_IO_Init();
	LCD_BL_IO_Init();
	
	spi_i2s_deinit(LCD_SPIx);
	
	rcu_periph_clock_enable(LCD_RCU_SPIx);
	spi_init(LCD_SPIx, &spi_init_struct);
	spi_enable(LCD_SPIx);
	
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

static void Set_SPI_DataSize_8b()
{
	//1.关闭SPI
	//spi_disable(LCD_SPIx);
	SPI_CTL0(LCD_SPIx) &= (uint32_t)(~SPI_CTL0_SPIEN);//直接寄存器操作
	
	
	//2.调整SPI参数
	#if(PORT_SPIx == 0) 
	{
		//注释:
		//#define SPI_FRAMESIZE_MASK         ((uint32_t)0x00000800U)  /*!< SPI0 frame size mask */
		//0:8位模式  1:16位模式
		SPI_CTL0(SPI0) &= ~((uint32_t)0x00000800U);
	}
	#elif(PORT_SPIx == 1)
	{
		#error("not support yet");
		//注释:
		//SPI_FIFO_INIT_MASK2   ((uint32_t)0x0000F0FFU)
		//SPI_BYTEN_MASK        ((uint32_t)0x00001000U)
		//SPI_CTL1(SPI1) = (SPI_CTL1(SPI1)&SPI_FIFO_INIT_MASK2)|SPI_FRAMESIZE_8BIT|SPI_BYTEN_MASK;
		SPI_CTL1(SPI1) = (SPI_CTL1(SPI1)&0xF0FFU)|SPI_FRAMESIZE_8BIT|0x1000U;
	}
	#endif
	
	//3.打开SPI
	//spi_enable(TFT_SPIx);
	SPI_CTL0(LCD_SPIx) |= (uint32_t)SPI_CTL0_SPIEN;//直接寄存器操作
}

static void Set_SPI_DataSize_16b()
{
	//1.关闭SPI
	//spi_disable(LCD_SPIx);
	SPI_CTL0(LCD_SPIx) &= (uint32_t)(~SPI_CTL0_SPIEN);//寄存器操作spi_disable(TFT_SPIx);
	
	//2.调整SPI参数
	#if(PORT_SPIx == 0) 
	{
		//注释:
		//#define SPI_FRAMESIZE_MASK         ((uint32_t)0x00000800U)  /*!< SPI0 frame size mask */
		//0:8位模式  1:16位模式
		SPI_CTL0(SPI0) |= ((uint32_t)0x00000800U);
	}
	#elif(PORT_SPIx == 1)
	{
		#error("not support yet");
		//注释:
		//SPI_FIFO_INIT_MASK2   ((uint32_t)0x0000F0FFU)
		//SPI_BYTEN_MASK        ((uint32_t)0x00001000U)
		//SPI_CTL1(SPI1) = (SPI_CTL1(SPI1)&(SPI_FIFO_INIT_MASK2&(~SPI_BYTEN_MASK)))|SPI_FRAMESIZE_16BIT;
		SPI_CTL1(SPI1) = (SPI_CTL1(SPI1)&((uint32_t)0x0000F0FFU&(~(uint32_t)0x00001000U)))|SPI_FRAMESIZE_16BIT;
	}
	#endif
	
	//3.打开SPI
	//spi_enable(LCD_SPIx);
	SPI_CTL0(LCD_SPIx) |= (uint32_t)SPI_CTL0_SPIEN;//直接寄存器操作
}

/*--------------------------------------------------------------
  * 名称: void LCD_Send_nCmd(uint16_t *p,uint16_t num)
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
		
		//3.设置SPI为8位模式
		Set_SPI_DataSize_8b();
			
		//4.开始传输第一个命令
		LCD_DC_Clr();
		LCD_CS_Clr();
		//spi_i2s_data_transmit(LCD_SPIx, p[0]);
		SPI_DATA(LCD_SPIx) = p[0];//直接寄存器操作

		//5.等待发送完成
		num--;
		while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
		while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	
		//6.发送剩下的数据
		LCD_DC_Set();
		while(num-->0)
		{
			p++;
			while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
			spi_i2s_data_transmit(LCD_SPIx, *p);
		}

		//7.等待发送完成
		while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
		while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
		
		//8.结束
		LCD_CS_Set();	
}
#endif

















/*--------------------------------------------------------------
  * 名称: LCD_Refresh(void)
  * 功能: 驱动接口,将显存LCD_GRAM全部内容发送至屏幕
----------------------------------------------------------------*/

//----------------------------普通OLED屏幕刷屏接口-------------------------------------
#if defined (LCD_USE_NORMAL_OLED)

//---------方式1:全屏刷新----------
#if (LCD_MODE == _FULL_BUFF_FULL_UPDATE)
uint8_t LCD_Refresh(void)
{
	uint8_t ypage;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		LCD_Set_Addr(0,ypage);
		LCD_Send_nDat(&lcd_driver.LCD_GRAM[ypage][0][0],SCREEN_WIDTH);
	}
	return 0;
}
//---------方式2:动态刷新----------
#elif (LCD_MODE == _FULL_BUFF_DYNA_UPDATE)
uint8_t LCD_Refresh(void)
{
	//每page做"sum+mini_crc组合"校验,若校验码没变,则不刷新该page
	unsigned char ypage;
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
	//每page做校验,若校验码没变,则不刷新该page
	uint8_t ypage;
	uint16_t x;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint32_t i_crc;

		//判断屏幕是否已刷完
		if(lcd_driver.lcd_refresh_ypage + ypage > ((SCREEN_HIGH+7)/8)-1)
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

#if (LCD_MODE == _FULL_BUFF_FULL_UPDATE)
//---------方式1:全缓存全屏刷新----------
uint8_t LCD_Refresh(void)
{
	unsigned char x,y,i;
	#if defined(GRAY_DRIVER_0DEG)
	//---扫描方向1(不旋转)----
	LCD_Set_Addr(0,0,(SCREEN_WIDTH/2-1),(SCREEN_HIGH-1));
	LCD_DC_Set();
	LCD_CS_Clr();
	for(y=0;y<SCREEN_HIGH;y++)
	{
		uint8_t page,mask;
		page=y/8;
		mask=(0x01<<y%8);
		for(x=0;x<SCREEN_WIDTH;)
		{
			i = 0x00;
			//第一个点
			if(lcd_driver.LCD_GRAM[page][x][0]&mask)
			{
				i = GRAY_COLOUR<<4;
			}
			x++;
			//第二个点
			if(x<SCREEN_WIDTH)
			{
				if(lcd_driver.LCD_GRAM[page][x][0]&mask)
				{
					i |= GRAY_COLOUR;
				}
			}
			x++;
			while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
			SPI_DATA(LCD_SPIx) = i;//spi_i2s_data_transmit(LCD_SPIx, p[i++]);
		}
	}

	#elif defined(GRAY_DRIVER_90DEG)
	//---扫描方向2(90度旋转)---
	LCD_Set_Addr(0,0,(SCREEN_HIGH/2-1),(SCREEN_WIDTH-1));
	LCD_DC_Set();
	LCD_CS_Clr();
	for(y=0;y<SCREEN_HIGH;y+=2)
	{
		uint8_t page1,page2,mask1,mask2;
		page1=y/8;
		page2=(y+1)/8;
		mask1=(0x01<<y%8);
		mask2=(0x01<<(y+1)%8);
		for(x=0;x<SCREEN_WIDTH;x++)
		{
			i = 0x00;
			//第一个点
			if(lcd_driver.LCD_GRAM[page1][x][0]&mask1)
			{
				i = GRAY_COLOUR<<4;
			}
			//第二个点
			if((y+1)<SCREEN_HIGH)
			{
				if(lcd_driver.LCD_GRAM[page2][x][0]&mask2)
				{
					i |= GRAY_COLOUR;
				}
			}
			while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
			SPI_DATA(LCD_SPIx) = i;//spi_i2s_data_transmit(LCD_SPIx, p[i++]);
		}
	}
	#endif
	while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_CS_Set();
	return 0;
}

#elif (LCD_MODE == _FULL_BUFF_DYNA_UPDATE)
//---------方式2:全缓存动态刷新----------
uint8_t LCD_Refresh(void)
{
	uint8_t ypage,y,i;
	uint16_t x,ycount;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint32_t i_crc;
		
		//-----方式1:CRC算法校验-----
		lcd_crc();//宏定义
		//---------------------------

		if(crc[ypage][0] != i_crc)
		{
			ycount = 8* ypage;
#if defined(GRAY_DRIVER_0DEG)
			//----扫描方向1(不旋转)---
			LCD_Set_Addr(0,ycount,(SCREEN_WIDTH/2-1),(SCREEN_HIGH-1));
			LCD_DC_Set();
			LCD_CS_Clr();
			
			for(y=0;y<8;y++)
			{
				uint8_t mask;
				if(++ycount >= SCREEN_HIGH){break;}
				mask=0x01<<(y%8);
				for(x=0;x<SCREEN_WIDTH;)
				{
					//----第一个点----
					i = 0x00;
					if(lcd_driver.LCD_GRAM[ypage][x][0]&mask)
					{
						i = GRAY_COLOUR<<4;
					}
					//----第二个点----
					x++;
					if((lcd_driver.LCD_GRAM[ypage][x][0]&mask)&&(x<SCREEN_WIDTH))
					{
						i |= GRAY_COLOUR;
					}
					x++;
					while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
					SPI_DATA(LCD_SPIx) = i;//spi_i2s_data_transmit(LCD_SPIx, p[i++]);
				}
			}

#elif defined(GRAY_DRIVER_90DEG)
			//---扫描方向2(90度旋转)----
			LCD_Set_Addr(ycount/2,0,(SCREEN_HIGH/2-1),(SCREEN_WIDTH-1));
			LCD_DC_Set();
			LCD_CS_Clr();
			for(y=0;y<8;y+=2)
			{
				if(++ycount >= SCREEN_HIGH){break;}
				for(x=0;x<SCREEN_WIDTH;x++)
				{
					//(4位一个点)
					//----第一个点----
					i = 0x00;
					if(lcd_driver.LCD_GRAM[ypage][x][0]&(0x01<<y))
					{
						i = GRAY_COLOUR<<4;
					}
					//----第二个点----
					if(lcd_driver.LCD_GRAM[ypage][x][0]&(0x01<<(y+1)))
					{
						i |= GRAY_COLOUR;
					}
					while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
					SPI_DATA(LCD_SPIx) = i;//spi_i2s_data_transmit(LCD_SPIx, p[i++]);
				}
			}
#endif
			crc[ypage][0] = i_crc;
			while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
			while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
			LCD_CS_Set();
		}
	}
	return 0;
}
//---------方式3:页缓存全局刷新----------
#elif (LCD_MODE == _PAGE_BUFF_FULL_UPDATE)
uint8_t LCD_Refresh(void)
{
	uint8_t i;
	uint16_t x,y;
	#if defined(GRAY_DRIVER_0DEG)
	//---扫描方向1(不旋转)----
	LCD_Set_Addr(0,lcd_driver.lcd_refresh_ypage*8,(SCREEN_WIDTH/2-1),(SCREEN_HIGH-1));
	LCD_DC_Set();
	LCD_CS_Clr();
	for(y=0;(y+lcd_driver.lcd_refresh_ypage*8)<SCREEN_HIGH;)
	{
		uint8_t page,mask;
		page=y/8;
		mask=(0x01<<y%8);
		for(x=0;x<SCREEN_WIDTH;)
		{
			i = 0x00;
			//第一个点
			if(lcd_driver.LCD_GRAM[page][x][0]&mask)
			{
				i = GRAY_COLOUR<<4;
			}
			x++;
			//第二个点
			if(x<SCREEN_WIDTH)
			{
				if(lcd_driver.LCD_GRAM[page][x][0]&mask)
				{
					i |= GRAY_COLOUR;
				}
			}
			x++;
			while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
			SPI_DATA(LCD_SPIx) = i;//spi_i2s_data_transmit(LCD_SPIx, p[i++]);
		}
		y++;
		if(y>=(GRAM_YPAGE_NUM*8))
		{
			break;
		}
	}

	#elif defined(GRAY_DRIVER_90DEG)
	//---扫描方向2(90度旋转)---
	LCD_Set_Addr(lcd_driver.lcd_refresh_ypage*4,0,(SCREEN_HIGH/2-1),(SCREEN_WIDTH-1));
	LCD_DC_Set();
	LCD_CS_Clr();
	for(y=0;(y+lcd_driver.lcd_refresh_ypage*8)<SCREEN_HIGH;)
	{
		uint8_t page1,page2,mask1,mask2;
		page1=y/8;
		page2=(y+1)/8;
		mask1=(0x01<<y%8);
		mask2=(0x01<<(y+1)%8);
		for(x=0;x<SCREEN_WIDTH;x++)
		{
			i = 0x00;
			//第一个点
			if(lcd_driver.LCD_GRAM[page1][x][0]&mask1)
			{
				i = GRAY_COLOUR<<4;
			}
			//第二个点
			if((y+1)<SCREEN_HIGH)
			{
				if(lcd_driver.LCD_GRAM[page2][x][0]&mask2)
				{
					i |= GRAY_COLOUR;
				}
			}
			while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
			SPI_DATA(LCD_SPIx) = i;//spi_i2s_data_transmit(LCD_SPIx, p[i++]);
		}
		y+=2;
		if(y>=(GRAM_YPAGE_NUM*8))
		{
			break;
		}
	}
	#endif
	while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_CS_Set();
	
	lcd_driver.lcd_refresh_ypage += GRAM_YPAGE_NUM;
	if(lcd_driver.lcd_refresh_ypage >= ((SCREEN_HIGH+7)/8))
	{
		lcd_driver.lcd_refresh_ypage = 0;
	}
	return lcd_driver.lcd_refresh_ypage;
}

//---------方式4:页缓存动态刷新----------
#elif (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE)
uint8_t LCD_Refresh(void)
{
	//每page做"sum+mini_crc组合"校验,若校验码没变,则不刷新该page
	uint8_t ypage,y,i;
	uint16_t x,ycount;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint32_t i_crc;
		//-----方式1:CRC算法校验-----
		lcd_crc();//宏定义
		//---------------------------
		
		if(crc[ypage][0] != i_crc)
		{
			ycount = 8* ypage;
#if defined(GRAY_DRIVER_0DEG)
			//----扫描方向1(不旋转)---
			LCD_Set_Addr(0,lcd_driver.lcd_refresh_ypage*8+ycount,(SCREEN_WIDTH/2-1),(SCREEN_HIGH-1));
			LCD_DC_Set();
			LCD_CS_Clr();
			
			for(y=0;y<8;y++)
			{
				uint8_t mask;
				ycount++;
				if((ycount >= SCREEN_HIGH)||(y>=(GRAM_YPAGE_NUM*8))){break;}

				mask=0x01<<(y%8);
				for(x=0;x<SCREEN_WIDTH;)
				{
					//----第一个点----
					i = 0x00;
					if(lcd_driver.LCD_GRAM[ypage][x][0]&mask)
					{
						i = GRAY_COLOUR<<4;
					}
					//----第二个点----
					x++;
					if((lcd_driver.LCD_GRAM[ypage][x][0]&mask)&&(x<SCREEN_WIDTH))
					{
						i |= GRAY_COLOUR;
					}
					x++;
					while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
					SPI_DATA(LCD_SPIx) = i;//spi_i2s_data_transmit(LCD_SPIx, p[i++]);
				}
			}

#elif defined(GRAY_DRIVER_90DEG)
			//---扫描方向2(90度旋转)----
			LCD_Set_Addr(lcd_driver.lcd_refresh_ypage*4 + ycount/2,0,((SCREEN_HIGH+1)/2-1),(SCREEN_WIDTH-1));
			LCD_DC_Set();
			LCD_CS_Clr();
			for(y=0;y<8;)
			{
				if(++ycount >= SCREEN_HIGH){break;}
				for(x=0;x<SCREEN_WIDTH;x++)
				{
					//(4位一个点)
					//----第一个点----
					i = 0x00;
					//j = y + lcd_driver.lcd_refresh_ypage*8;
					//if((j+1)<=SCREEN_HIGH)
					{
						if(lcd_driver.LCD_GRAM[ypage][x][0]&(0x01<<y))
						{
							i = GRAY_COLOUR<<4;
						}
						//----第二个点----
						//if((j+1)<SCREEN_HIGH)
						{
							if(lcd_driver.LCD_GRAM[ypage][x][0]&(0x01<<(y+1)))
							{
								i |= GRAY_COLOUR;
							}
						}
					}
					while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
					SPI_DATA(LCD_SPIx) = i;//spi_i2s_data_transmit(LCD_SPIx, p[i++]);
				}
				y+=2;
			}
#endif
			crc[ypage][0] = i_crc;
			while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
			while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
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



//----------------------------RGB565屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_RGB565)



#if (LCD_MODE == _FULL_BUFF_FULL_UPDATE)
//---------方式1:全缓存全屏刷----------
uint8_t LCD_Refresh(void)
{
	uint16_t x,y;
	LCD_Set_Addr(0,0,SCREEN_WIDTH-1,SCREEN_HIGH-1);
	
	Set_SPI_DataSize_16b();
	LCD_DC_Set();
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
			while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET){}//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
			SPI_DATA(LCD_SPIx) = lcd_driver.colour[c];//spi_i2s_data_transmit(LCD_SPIx, lcd_driver.colour[c]);
		}
	}
	//while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_CS_Set();
	Set_SPI_DataSize_8b();
	return 0;
}

#elif (LCD_MODE == _FULL_BUFF_DYNA_UPDATE)
//---------方式2:全缓存动态刷----------
uint8_t LCD_Refresh(void)
{
	//每page做校验,若校验码没变,则不刷新该page
	uint8_t ypage;
	uint16_t x,ycount;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint16_t i_crc;
		uint8_t i_dyna_count;//每页检测DYNA_CRC_NUM次
		for(i_dyna_count=0;i_dyna_count<PAGE_CRC_NUM;i_dyna_count++)
		{
			uint16_t x_start = i_dyna_count * DYNA_CRC_ONCE_XNUM;
			uint16_t x_end   = x_start + DYNA_CRC_ONCE_XNUM ;
			uint8_t *i = &lcd_driver.LCD_GRAM[ypage][x_start][0];
			if(x_end > (SCREEN_WIDTH-1)){x_end = SCREEN_WIDTH-1;}
			//--------CRC算法校验--------
			CRC_CTL |= (uint32_t)CRC_CTL_RST;
			for(x=0;x<=(x_end-x_start)*LCD_COLOUR_BIT;x++)
			{
				REG8(CRC) = *i++;
			}
			i_crc = (uint16_t)CRC_DATA;
			//---------------------------
		
			if(crc[ypage][i_dyna_count] != i_crc)
			{
				uint8_t y=0;
				
				LCD_Set_Addr(x_start,(ypage)*8,x_end,SCREEN_HIGH-1);
				LCD_DC_Set();
				LCD_CS_Clr();
				Set_SPI_DataSize_16b();
				
				ycount = 8* (ypage);
				for(y=0;y<8;y++)
				{
					uint8_t mask;
					if(++ycount > SCREEN_HIGH)
					{
						break;
					}
					mask=0x01<<(y%8);
					uint8_t *gram=&lcd_driver.LCD_GRAM[ypage][x_start][0];
					for(x=0;x<=(x_end-x_start);x++)
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
						while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
						SPI_DATA(LCD_SPIx) = lcd_driver.colour[c];//spi_i2s_data_transmit(LCD_SPIx, lcd_driver.colour[c]);
					}
				}
				crc[ypage][i_dyna_count] = i_crc;
				//while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
				while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
				LCD_CS_Set();
				Set_SPI_DataSize_8b();
			}
		}
	}
	LCD_CS_Set();
	Set_SPI_DataSize_8b();
	return 0;
}
#elif (LCD_MODE == _PAGE_BUFF_FULL_UPDATE)

//---------方式3:页缓存全屏刷新----------
uint8_t LCD_Refresh(void)
{
	uint16_t x,y;
	LCD_Set_Addr(0,lcd_driver.lcd_refresh_ypage*8,SCREEN_WIDTH-1,SCREEN_HIGH-1);
	
	Set_SPI_DataSize_16b();
	LCD_DC_Set();
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
			while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
			SPI_DATA(LCD_SPIx) = lcd_driver.colour[c];//spi_i2s_data_transmit(LCD_SPIx, lcd_driver.colour[c]);
		}
		y++;
		if(y>=(GRAM_YPAGE_NUM*8))
		{
			break;
		}
	}
	//while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_CS_Set();
	Set_SPI_DataSize_8b();
	
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
	uint8_t ypage;
	uint16_t x,ycount;
	for(ypage=0;ypage<GRAM_YPAGE_NUM;ypage++)
	{
		uint16_t i_crc;
		uint8_t i_dyna_count;//每页检测DYNA_CRC_NUM次
		if((lcd_driver.lcd_refresh_ypage + ypage)>=((SCREEN_HIGH+7)/8))
		{
			break;
		}
		for(i_dyna_count=0;i_dyna_count<PAGE_CRC_NUM;i_dyna_count++)
		{
			uint16_t x_start = i_dyna_count * DYNA_CRC_ONCE_XNUM;
			uint16_t x_end   = x_start + DYNA_CRC_ONCE_XNUM ;
			uint8_t *i = &lcd_driver.LCD_GRAM[ypage][x_start][0];
			if(x_end > (SCREEN_WIDTH-1)){x_end = SCREEN_WIDTH-1;}
			//--------CRC算法校验--------
			CRC_CTL |= (uint32_t)CRC_CTL_RST;
			for(x=0;x<=(x_end-x_start)*LCD_COLOUR_BIT;x++)
			{
				REG8(CRC) = *i++;
			}
			i_crc = (uint16_t)CRC_DATA;
			//---------------------------
			
			if(crc[lcd_driver.lcd_refresh_ypage + ypage][i_dyna_count] != i_crc)
			{
				uint8_t y=0;
				
				LCD_Set_Addr(x_start,(lcd_driver.lcd_refresh_ypage+ypage)*8,x_end,SCREEN_HIGH-1);
				LCD_DC_Set();
				LCD_CS_Clr();
				Set_SPI_DataSize_16b();
				
				ycount = 8* (ypage + lcd_driver.lcd_refresh_ypage);
				for(y=0;y<8;y++)
				{
					uint8_t mask;
					if(++ycount > SCREEN_HIGH)
					{
						break;
					}
					mask=0x01<<(y%8);
					uint8_t *gram=&lcd_driver.LCD_GRAM[ypage][x_start][0];
					for(x=0;x<=(x_end-x_start);x++)
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
						while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
						SPI_DATA(LCD_SPIx) = lcd_driver.colour[c];//spi_i2s_data_transmit(LCD_SPIx, lcd_driver.colour[c]);
					}
				}
				crc[lcd_driver.lcd_refresh_ypage + ypage][i_dyna_count] = i_crc;
				//while((SPI_STAT(LCD_SPIx) & SPI_FLAG_TBE)==RESET);//while(spi_i2s_flag_get(LCD_SPIx,SPI_FLAG_TBE)==RESET);
				while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
				LCD_CS_Set();
				Set_SPI_DataSize_8b();
			}
		}
	}
	Set_SPI_DataSize_8b();
	
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
