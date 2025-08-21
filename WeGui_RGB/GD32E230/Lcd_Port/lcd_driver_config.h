/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/
#ifndef LCD_DRIVER_CONFIG_H
#define LCD_DRIVER_CONFIG_H

/*--------------------------------------------------------------
  * Wegui : V0.5
  * Author: KOUFU
	* https://space.bilibili.com/526926544
	* https://github.com/KOUFU-DIY/WeGui_RGB
----------------------------------------------------------------*/
/*--------------------------------------------------------------
	* 版本更新 : WeGui RGB V0.5.4
	* 1.新增codeblock的PC仿真
	* 2.修复菜单的一些问题
	* 3.大幅减少菜单ram占用
	* 4.部分函数功能修复
----------------------------------------------------------------*/


//提示: 

//该工程支持四种工作模式, 内存充足情况下建议使用(2), 内存不足建议使用(4), 工作方式在下方通过宏定义调整切换
//(1)"全缓存全屏刷新",最原始的刷新模式
//(2)"全缓存动态刷新",最快的刷新模式,全屏动画时与(1)的刷屏速度基本一致
//(3)"页缓存全屏刷新",节省内存的原始刷新模式
//(4)"页缓存动态刷新",节省内存且刷得快得模式, 全屏动画时与(3)的刷屏速度基本一致


//字库及其他图片资源在lcd_res,可自行使用配套工具修改裁剪多国语言字库和图片取模


/*------------------------------------------------------------------------------------
	* ----默认驱动接口----        ---默认按键接口---          外接ADC电位器接口 A1
	* |--SPI--|  |--IIC--|       |OK   | A0  |确定|         (用于DEMO外部数据演示)
	* |BL   B0|  |RES  A6|       |BACK | C15 |返回|          
	* |CS   A6|  |SDA B11|       |UP   | B13 | 上 |         板载闪烁LED  C13
	* |DC  B11|  |SCL B10|       |DOWN | B14 | 下 |    
	* |RES B10|  |3V3 3V3|       |LEFT | B15 | 左 |         UART_RX  A3 
	* |SDA  A7|  |GND GND|       |RIGHT| B12 | 右 |         UART_TX  A2
	* |SCL  A5|                                       
	* |3V3 3V3|  
	* |GND GND|
-------------------------------------------------------------------------------------*/






//-------------------------1.选择一个屏幕通讯接口-----------------------------

#define _SOFT_3SPI  (0)//软件三线SPI驱动   对应文件gd32e230_lcd_soft_3spi_port.c
#define _SOFT_4SPI  (1)//软件四线SPI驱动   对应文件gd32e230_lcd_soft_4spi_port.c
#define _HARD_4SPI  (2)//硬件四线SPI0驱动   对应文件gd32e230_lcd_hard_4spi_port.c
#define _DMA_4SPI   (4)//DMA四线SPI驱动   对应文件gd32e230_lcd_dma_4spi_port.c
#define _SOFT_I2C   (5)//软件IIC驱动 对应文件gd32e230_lcd_soft_i2c_port.c 
#define _HARD_I2C   (6)//硬件IIC驱动 对应文件gd32e230_lcd_hard_i2c_port.c
#define _OTHER_PORT (7)//项目自定义 对应文件gd32e230_lcd_other_port.c
#define LCD_PORT    _HARD_4SPI//选择一个接口


//----------------------------2.设定屏幕分辨率--------------------------------
#define SCREEN_WIDTH 320  //建议取8的倍数
#define SCREEN_HIGH  172  //建议取8的倍数

//---------------------------3.1设定屏幕IIC--------------------------------
#if ((LCD_PORT == _HARD_I2C) || (LCD_PORT == _SOFT_I2C) || (LCD_PORT == _OTHER))

	//设置屏幕I2C地址
	#define OLED_IIC_7ADDR 0x3C //7位0x3C => 8位0x78 (大部分默认)
	//#define OLED_IIC_7ADDR 0x3D //7位0x3D => 8位0x7A


	//设置硬件I2C速率(手册标注最大允许1MHz,实测也支持超频)
	//#define I2C_FREQ_INIT (2000000) //自定义
	//#define I2C_FREQ_INIT 100000 //100kHz标速
	#define I2C_FREQ_INIT 400000 //400kHz快速
	//#define I2C_FREQ_INIT 1000000 //1MHz快速+
	

#endif 

//-------------------------3.2设定硬件SPI时钟速率-------------------------------
#if ((LCD_PORT == _HARD_4SPI) || (LCD_PORT == _DMA_4SPI) || (LCD_PORT == _OTHER_PORT))

//设置硬件SPI分频
//GD32E230手册描述SPI最高设置18MHz 实测支持超频(72MHz/SPI_PSC_4=18MHz)
#define SPI_PSC_x       SPI_PSC_2 
#endif


//---------------------------4.1.选择刷屏方式--------------------------------
#define _FULL_BUFF_FULL_UPDATE (0) //全屏缓存 全屏刷新[暂不支持上位机串流]
#define _FULL_BUFF_DYNA_UPDATE (1) //全屏缓存 动态刷新(更高的刷新速度)[暂不支持上位机串流]
#define _PAGE_BUFF_FULL_UPDATE (2) //页缓存 全屏刷新(更低的内存占用)[暂不支持上位机串流和截屏]
#define _PAGE_BUFF_DYNA_UPDATE (3) //页缓存 动态刷新(更低的内存占用,建议使用)[暂不支持上位机串流和截屏]

#define LCD_MODE    _PAGE_BUFF_DYNA_UPDATE //选择一个刷屏模式

//-------------------------4.2.设置择刷屏缓存页大小--------------------------------
//仅页缓存模式需要设置
#if ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
	#define GRAM_YPAGE_NUM (4)//自定义 最小取1 最大取(((SCREEN_HIGH+7)/8))
	//#define GRAM_YPAGE_NUM ((((SCREEN_HIGH+7)/8)+1)/2)//设置二分之一屏缓存
	//#define GRAM_YPAGE_NUM ((((SCREEN_HIGH+7)/8)+3)/4)//设置四分之一屏缓存
	//#define GRAM_YPAGE_NUM ((((SCREEN_HIGH+7)/8)+7)/8)//设置八分之一屏缓存
	//#define GRAM_YPAGE_NUM (((SCREEN_HIGH+7)/8))//设置全屏缓存(请直接使用_FULL_BUFF_xxxx_UPDATE模式获取更高的性能)
#endif

//-------------------------5.3.设置择刷屏缓存页大小--------------------------------
//仅适配了TFT彩屏 //每页连续横向扫描DYNA_CRC_ONCE_NUM个像素校验一个crc
#if ((LCD_MODE == _PAGE_BUFF_DYNA_UPDATE)||(LCD_MODE == _FULL_BUFF_DYNA_UPDATE))
	#define PAGE_CRC_NUM  (8)     //动态刷新每页细分校验的分区 默认1 仅TFT可用,其他类型屏幕默认取1
#endif






//-----------------------5.设定屏幕区域显示偏移设置--------------------------
//oled屏幕优先修改驱动初始化
#define SCREEN_X_OFFSET     (0) //x左右方向偏移像素
//#define SCREEN_Y_OFFSET     (0) //y上下方向偏移像素
#define SCREEN_Y_OFFSET     (34) //y上下方向偏移像素

//-----------------------------6.1选择屏幕IC---------------------------------
#define _SH1106    (0)//普通点阵OLED
#define _SH1108    (1)//普通点阵OLED
#define _SH1107    (2)//普通点阵OLED
#define _SH1115    (3)//普通点阵OLED
#define _SSD1306   (4)//普通点阵OLED (最常用)
#define _SSD1309   (5)//普通点阵OLED 
#define _SSD1312   (6)//普通点阵OLED
#define _SSD1315   (7)//普通点阵OLED
#define _SSD1327   (8)//灰度OLED
#define _ST7735    (9)//TFT彩屏 RGB565
#define _ST7789V3 (10)//TFT彩屏 RGB565
#define _ST7789VW (11)//TFT彩屏 RGB565
#define _ST7796S  (12)//TFT彩屏 RGB565
#define _GC9A01   (13)//TFT彩屏 RGB565 (未完善)
//#define _ILI9488  (14)//TFT彩屏 RGB111 RGB888 暂不支持

#define LCD_IC _ST7789V3 //选择一个屏幕IC型号

//---------------------------6.2设定灰度屏--------------------------------
//仅"灰度OLED屏"需要设置
//注意目前驱动限制,屏幕宽高须为8的倍数,否则屏幕可能会有溢出

//1.选择一个灰度屏扫描方向[需要与初始化(A0指令)匹配]
#define GRAY_DRIVER_0DEG  //方向1
//#define GRAY_DRIVER_90DEG //方向2

//2.设置画笔灰度亮度[1:15]
#define GRAY_COLOUR  15 //画笔灰度(亮度)



















  
 

//------------编译-----------

#if (LCD_PORT == _SOFT_3SPI)    //软件三线SPI
	#include "gd32e230_lcd_soft_3spi_port.h"
#elif (LCD_PORT == _SOFT_4SPI) //软件四线SPI 
	#include "gd32e230_lcd_soft_4spi_port.h"
#elif (LCD_PORT == _HARD_4SPI) //硬件四线SPI 
	#include "gd32e230_lcd_hard_4spi_port.h"
#elif (LCD_PORT == _DMA_4SPI) //DMA四线SPI 
	#include "gd32e230_lcd_dma_4spi_port.h"
#elif (LCD_PORT == _SOFT_I2C)  //软件I2C   
	#include "gd32e230_lcd_soft_i2c_port.h"
#elif (LCD_PORT == _HARD_I2C)  //硬件I2C   
	#include "gd32e230_lcd_hard_i2c_port.h"
#elif (LCD_PORT == _DMA_IIC)  //DMA_IIC驱动
	#error ("gd32e230 dma iic driver is not supported yet")
#elif (LCD_PORT == _OTHER_PORT) //项目自定义驱动
	#include "gd32e230_lcd_other_port.h"
#endif

//全屏缓存,固定大小
#if ((LCD_MODE == _FULL_BUFF_FULL_UPDATE) || (LCD_MODE == _FULL_BUFF_DYNA_UPDATE)) 
	#define GRAM_YPAGE_NUM ((SCREEN_HIGH+7)/8)
#elif ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
	#if (GRAM_YPAGE_NUM >= ((SCREEN_HIGH+7)/8))
		#undef GRAM_YPAGE_NUM
		#define GRAM_YPAGE_NUM ((SCREEN_HIGH+7)/8)
		#undef LCD_MODE
		#define LCD_MODE _FULL_BUFF_FULL_UPDATE
		#warning("Buff enough. Use _FULL_BUFF_FULL_UPDATE mode now!")
	#endif
#endif

#if (LCD_IC == _SH1106)
	#define LCD_USE_NORMAL_OLED
	#define LCD_IC_Init() do{SH1106_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SH1106_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SH1106_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SH1106_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{SH1106_Set_Contrast(x);}while(0)
	#include "sh1106.h"
#elif (LCD_IC == _SH1108)
	#define LCD_USE_NORMAL_OLED
	#define LCD_IC_Init() do{SH1108_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SH1108_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SH1108_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SH1108_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{SH1108_Set_Contrast(x);}while(0)
	#include "sh1108.h"
#elif (LCD_IC == _SH1107)
	#define LCD_USE_NORMAL_OLED
	#define LCD_IC_Init() do{SH1107_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SH1107_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SH1107_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SH1107_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{LCD_Send_1Cmd(0x81);LCD_Send_1Cmd(x);}while(0)
	#include "SH1107.h"
#elif (LCD_IC == _SH1115)
	#define LCD_USE_NORMAL_OLED
	#define LCD_IC_Init() do{SH1115_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SH1115_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SH1115_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SH1115_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{SH1115_The_Contrast_Control_Mode_Set(x);}while(0)
	#include "sh1115.h"
#elif (LCD_IC == _SSD1306)
	#define LCD_USE_NORMAL_OLED
	#define LCD_IC_Init() do{SSD1306_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SSD1306_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SSD1306_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SSD1306_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{LCD_Send_1Cmd(0x81);LCD_Send_1Cmd(x);}while(0)
	#include "ssd1306.h"
#elif (LCD_IC == _SSD1309)
	#define LCD_USE_NORMAL_OLED
	#define LCD_IC_Init() do{SSD1309_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SSD1309_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SSD1309_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SSD1309_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{LCD_Send_1Cmd(0x81);LCD_Send_1Cmd(x);}while(0)
	#include "ssd1309.h"
#elif (LCD_IC == _SSD1312)
	#define LCD_USE_NORMAL_OLED
	#define LCD_IC_Init() do{SSD1312_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SSD1312_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SSD1312_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SSD1312_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{LCD_Send_1Cmd(0x81);LCD_Send_1Cmd(x);}while(0)
	#include "ssd1312.h"
#elif (LCD_IC == _SSD1315)
	#define LCD_USE_NORMAL_OLED
	#define LCD_IC_Init() do{SSD1315_Init();}while(0)
	#define LCD_Set_Addr_x(x) do{SSD1315_Set_Address_x(x+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_ypage(page) do{SSD1315_Set_Address_ypage(page+SCREEN_Y_OFFSET/8);}while(0)
	#define LCD_Set_Addr(x,page) do{SSD1315_Set_Address_x_ypage((x+SCREEN_X_OFFSET),(page+SCREEN_Y_OFFSET/8));}while(0)
	#define LCD_Set_Bright(x) do{SSD1315_Set_Contrast_Control(x);}while(0)
	#include "ssd1315.h"
#elif (LCD_IC == _SSD1327)
	#define LCD_USE_GRAY_OLED
	#define LCD_IC_Init() do{SSD1327_Init();}while(0)
	#define LCD_Set_Addr_x(x0,x1) do{SSD1327_Set_Addr_x(x0+SCREEN_X_OFFSET,x1+SCREEN_X_OFFSET);}while(0)
	#define LCD_Set_Addr_y(y0,y1) do{SSD1327_Set_Addr_y(y0+SCREEN_Y_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{SSD1327_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{/*SSD1327_Set_Contrast_Control(x);*/}while(0)
	#include "ssd1327.h"
#elif (LCD_IC == _ST7735)
	#define LCD_USE_RGB565
	#define LCD_IC_Init() do{ST7735_Init();}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{ST7735_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{}while(0)
	#include "st7735.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _ST7789V3)
	#define LCD_USE_RGB565
	#define LCD_IC_Init() do{ST7789V3_Init();}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{ST7789V3_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{}while(0)
	#include "st7789v3.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _ST7789VW)
	#define LCD_USE_RGB565
	#define LCD_IC_Init() do{ST7789VW_Init();}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{ST7789VW_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{}while(0)
	#include "st7789vw.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _ST7796S)
	#define LCD_USE_RGB565
	#define LCD_IC_Init() do{ST7796S_Init();}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{ST7796S_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{}while(0)
	#include "st7796s.h"
	#include "TFT_Color.h"
#elif (LCD_IC == _GC9A01)
	#define LCD_USE_RGB565
	#define LCD_IC_Init() do{GC9A01_Init();}while(0)
	#define LCD_Set_Addr(x0,y0,x1,y1) do{GC9A01_Set_Addr(x0+SCREEN_X_OFFSET,y0+SCREEN_Y_OFFSET,x1+SCREEN_X_OFFSET,y1+SCREEN_Y_OFFSET);}while(0)
	#define LCD_Set_Bright(x) do{}while(0)
	#include "gc9a01.h"
	#include "TFT_Color.h"
#else
	#warning("No screen ic init!")
#endif





#if !defined LCD_USE_RGB565//仅适配了TFT
	#if defined PAGE_CRC_NUM
		#undef PAGE_CRC_NUM
	#endif
#endif

#if !defined PAGE_CRC_NUM
		#define PAGE_CRC_NUM (1)
#endif

#define DYNA_CRC_ONCE_XNUM  ((SCREEN_WIDTH+PAGE_CRC_NUM-1)/PAGE_CRC_NUM) //一次校验的数量

	
#endif
	
