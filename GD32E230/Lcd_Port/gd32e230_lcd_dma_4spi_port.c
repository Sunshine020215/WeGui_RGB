/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#include "lcd_driver_config.h"
#include "lcd_wegui_config.h"

#if(LCD_PORT == _DMA_4SPI)


#include "gd32e230_lcd_dma_4spi_port.h"
#include "lcd_driver.h"
#include "stdint.h"

//配置SPI通道
#define PORT_SPIx       (0)
#define LCD_SPIx        SPI0
#define LCD_RCU_SPIx    RCU_SPI0

//配置DMA通道及其中断
#define LCD_DMA_CHx							DMA_CH2 //SPI0_TX处于DMA_CH2 //SPI1_TX处于DMA_CH4
#define LCD_RCU_DMAx 						RCU_DMA
#define LCD_DMAx_IRQn 					DMA_Channel1_2_IRQn //中断函数通道
#define LCD_DMAx_INT_Func				DMA_Channel1_2_IRQHandler //中断函数名称

volatile lcd_dma_step_t  DMA_State = DMA_FREE;
uint8_t DMA_reflash_step=0;


///*--------------------------------------------------------------
//  * 名称: LCD_is_Busy()
//  * 返回: 0:DMA空闲 1:DMA忙碌
//  * 功能: 
//  * 说明: 
//----------------------------------------------------------------*/
//uint8_t LCD_is_Busy()
//{
//	return DMA_State;
//}



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
	
	
	
		dma_parameter_struct dma_init_struct =
		{
			.periph_addr  = (uint32_t)&SPI_DATA(LCD_SPIx),
			.memory_addr  = (uint32_t)lcd_driver.LCD_GRAM,
			.direction    = DMA_MEMORY_TO_PERIPHERAL,
			.memory_width = DMA_MEMORY_WIDTH_8BIT,
			.periph_width = DMA_PERIPHERAL_WIDTH_8BIT,
			.priority     = DMA_PRIORITY_LOW,
			.number       = 0,
			.periph_inc   = DMA_PERIPH_INCREASE_DISABLE,
			.memory_inc   = DMA_MEMORY_INCREASE_ENABLE
		};
		rcu_periph_clock_enable(RCU_DMA);
		
		/* configure SPI0 transmit DMA:TFT_DMAx-TFT_DMA_CHx */
		dma_deinit(LCD_DMA_CHx);
		dma_init(LCD_DMA_CHx, (dma_parameter_struct*)&dma_init_struct);
			
		/* configure DMA mode */
		dma_circulation_disable(LCD_DMA_CHx);
		dma_memory_to_memory_disable(LCD_DMA_CHx);
			
		spi_dma_enable(LCD_SPIx, SPI_DMA_TRANSMIT);
			
		nvic_irq_enable(LCD_DMAx_IRQn, 0);//开中断
			
		dma_interrupt_enable(LCD_DMA_CHx,DMA_INT_FTF);
		//dma_channel_enable(LCD_DMA_CHx);

	
	
	#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE ==_PAGE_BUFF_DYNA_UPDATE))
	rcu_periph_clock_enable(RCU_CRC);//动态刷新CRC校验用
	#endif

	DMA_State=DMA_FREE;
	
	LCD_RES_Clr();
	{
		volatile unsigned int i=10000;
		while(i--);
	}
	LCD_RES_Set();
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

/*--------------------------------------------------------------
  * 名称: LCD_Send_1Cmd(uint8_t dat)
  * 传入1: dat
  * 返回: 无
  * 功能: 向屏幕发送1个命令(DC=0时发1字节spi)
  * 说明: 
----------------------------------------------------------------*/
void LCD_Send_1Cmd(uint8_t dat)
{
	while(DMA_State!=DMA_FREE){;}
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
	while(DMA_State!=DMA_FREE){;}
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
	while(DMA_State!=DMA_FREE){;}
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_DC_Set();
	LCD_CS_Clr();
		
	//1.关闭DMA(很多情况下关闭DMA才能调整DMA参数)
	//dma_channel_disable(LCD_DMA_CHx);//关闭DMA(关闭DMA才能调整DMA参数)
	DMA_CHCTL(LCD_DMA_CHx) &= ~DMA_CHXCTL_CHEN;//直接寄存器操作
	
	//2.设置DMA待发送的数据数量,连续发多个数据
	//dma_transfer_number_config(LCD_DMA_CHx , num);
	DMA_CHCNT(LCD_DMA_CHx) = (num & DMA_CHANNEL_CNT_MASK);//直接寄存器操作
		
	//3.设置DMA待发送的数据的起始地址指针
	dma_memory_address_config(LCD_DMA_CHx, (uint32_t)p);
	//DMA_CHMADDR(LCD_DMA_CHx) = (uint32_t)p;//直接寄存器操作
	
	//4.开启DMA传输
	//dma_channel_enable(TFT_DMA_CHx);
	DMA_CHCTL(LCD_DMA_CHx) |= DMA_CHXCTL_CHEN;//直接寄存器操作

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
	while(DMA_State!=DMA_FREE){;}
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

#endif












/*--------------------------------------------------------------
  * 名称: LCD_Refresh(void)
  * 功能: 驱动接口,将显存LCD_GRAM全部内容发送至屏幕
----------------------------------------------------------------*/
/*--------------------------------------------------------------
  * 名称: LCD_DMAx_INT_Func(void)
  * 功能: DMA中断接口
----------------------------------------------------------------*/

//----------------------------普通OLED屏幕刷屏接口-------------------------------------
#if defined (LCD_USE_NORMAL_OLED)

#if (LCD_MODE == _FULL_BUFF_FULL_UPDATE)
//---------方式1:全屏刷新----------
//--优点:全屏刷新
//--缺点:内容不变的区域也参与了刷新
uint8_t LCD_Refresh(void)
{
	while(DMA_State != DMA_FREE){;}
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_Set_Addr(0,0);
	while(DMA_State != DMA_FREE){;}
	DMA_State = DMA_REFLASH;
	DMA_reflash_step=1;
		
	//发送第一行,激活dma  方式1,LCD_Send_nDat函数发送 因中断需要用DMA_State而提前修改了值, 此时DMA_State不为FREE, 直接用该函数会进入死循环
	//LCD_Send_nDat(&lcd_driver.LCD_GRAM[0][0][0],SCREEN_WIDTH);
	
		
		
	//发送第一行,激活dma  方式2,将LCD_Send_nDat函数拆散,去除DMA_State不为FREE的判断
	while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
	LCD_DC_Set();
	LCD_CS_Clr();
	DMA_CHCTL(LCD_DMA_CHx) &= ~DMA_CHXCTL_CHEN;
	DMA_CHCNT(LCD_DMA_CHx) = (SCREEN_WIDTH & DMA_CHANNEL_CNT_MASK);
	dma_memory_address_config(LCD_DMA_CHx, (uint32_t)&lcd_driver.LCD_GRAM[0][0][0]);
	DMA_CHCTL(LCD_DMA_CHx) |= DMA_CHXCTL_CHEN;
	

	//DMA中断里刷新余下行
	return 0;
}

void LCD_DMAx_INT_Func()
{
		lcd_dma_step_t save_DMA_State=DMA_State;
		save_DMA_State=DMA_State;
		DMA_State = DMA_FREE;//解决中断里因该变量引起死循环的问题
	
	
		DMA_CHCTL(LCD_DMA_CHx) &= ~DMA_CHXCTL_CHEN;
		dma_interrupt_flag_clear(LCD_DMA_CHx,DMA_INT_FLAG_G);
	
		//等待SPI发完(DMA完毕不代表SPI完毕)
		while(SPI_STAT(LCD_SPIx) & SPI_FLAG_TRANS);//while(spi_i2s_flag_get(TFT_SPIx,SPI_FLAG_TRANS));
		LCD_CS_Set();
	
		switch(save_DMA_State)
		{
			case DMA_FREE:break;
			case DMA_NORMAL_CMD:
			{
				save_DMA_State = DMA_FREE;
				LCD_CS_Set();
			}break;
			case DMA_REFLASH:
			{
				if(DMA_reflash_step >= GRAM_YPAGE_NUM)
				{
					LCD_CS_Set();
					DMA_reflash_step = 0;
					save_DMA_State = DMA_FREE;
					break;
				}
				LCD_Set_Addr(0,DMA_reflash_step);
				LCD_Send_nDat(&lcd_driver.LCD_GRAM[DMA_reflash_step++][0][0],SCREEN_WIDTH);
			}break;
		}
		DMA_State = save_DMA_State;
}

#elif (LCD_MODE == _FULL_BUFF_DYNA_UPDATE)
	//动态刷新不支持dma方式
	#error("DYNA_UPDATE mode not support dma4spi driver yet!")
#elif (LCD_MODE == _PAGE_BUFF_FULL_UPDATE)
	//页刷新不支持dma方式
	#error("PAGE_BUFF mode not support dma4spi driver yet!")
#elif (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE)
	//页刷新不支持dma方式
	#error("PAGE_BUFF mode not support dma4spi driver yet!")
#endif

//----------------------------灰度OLED屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_GRAY_OLED)//灰度OLED
	//灰度OLED屏不支持dma4spi方式驱动 请更改屏幕驱动方式
	#error ("Gray OLED not support dma4spi driver yet!Use 4spi_driver please!")

//----------------------------RGB565屏幕刷屏接口-------------------------------------
#elif defined (LCD_USE_RGB565)
	//彩屏TFT屏不支持dma4spi方式驱动 请更改屏幕驱动方式
	#error ("TFT not support dma4spi driver yet!");
#else
	#error ("Not support LCD!");
#endif



#endif
