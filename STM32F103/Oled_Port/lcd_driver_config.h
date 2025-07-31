#ifndef LCD_DRIVER_CONFIG_H
#define LCD_DRIVER_CONFIG_H
/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

/*--------------------------------------------------------------
  * Wegui : V0.5
  * Author: KOUFU
	* https://space.bilibili.com/526926544
	* https://github.com/KOUFU-DIY/WeGui_OLED
----------------------------------------------------------------*/
/*--------------------------------------------------------------
	* 版本更新 : WeGui RGB V0.5.1
	* 1.修复页缓存大小不对齐,动态刷新模式的数据溢出问题!
	* 2.修复屏幕分辨率宽度大于255引起的问题
	* 3.修复进退菜单的一些问题
	
  * 版本更新 : WeGui RGB V0.5 
	* WeGui OLED进阶版:WeGui RGB初版发布!
	* 1. 修复OLED在使用动态刷新时, 上电屏幕有残留的问题
	* 2. 完善"关于"和"开发者选项"列表中
	* 3. 新增TFT屏幕的背光调节功能
	* 4. 在OLED基础上增加了彩屏RGB多色玩法,持更多配色主题,更佳的视觉效果
	
	* 5.1 RGB版本基继承了OLED的所有功能, 部分功能作修改
	* 5.2 OLED原本在write_1模式下执行Lcd_Clear_GRAM();显存会写0xff,
	* 5.3 OLED原本在write_0模式下执行Lcd_Clear_GRAM();显存会写0x00,
	* 5.4 现RGB在任何模式下,执行Lcd_Clear_GRAM();时显存会写0x00,
	* 5.5 write_0更改位"笔刷0" 笔刷颜色由lcd_driver.colour[0]决定
	* 5.6 write_1更改位"笔刷1" 笔刷颜色由lcd_driver.colour[1]决定
	* 5.7 同时在2色模式中增加write_2和write_3
	* 5.8 同时在3色模式中增加write_2,write_3,write_4...,write_7
	* 5.9 建议使用128x160 TFT屏幕获取最佳的适配体验
	* 5.10上位机串流功能暂未完全适配

----------------------------------------------------------------*/

/*--------------------------------------------------------------
	* 暂不支持使用ARMV6编译,请使用ARMV5编译器(默认)
	* 若keil版本过高,请自行安装v5版本编译器
	* 本程序使用了大量的"UTF8"支持多国语言的编码
	* 需要进行两步设置
	* 1.Edit->Configuration->Encoding->"Encod in UTF8"
	* 2.Project -> Oprions for Target-> C/C++ -> 
	* Misc Contiols -> 填入"--locale=english"
----------------------------------------------------------------*/
//提示: 
//IIC相关功能暂不支持使用ARMV6编译,请使用ARMV5编译器!!
//该芯片IIC存在"缺陷",其怪点暂未解决,暂未适配其DMA驱动方式, 推荐使用软件IIC方式!!
//IIC硬件在一次通讯失败后,将拖慢系统运行,暂未解决, 推荐使用软件IIC方式!!
//软件IIC默认使用适中的延迟速率,需要更快的刷新速度,需要到对应port文件里修改, 查找I2C_SCL_Rise_Delay等并进行修改延迟值
//使用模拟IIC可精确控制上升和下降时间,调整合适的延迟时间,刷新率可以比硬件IIC更快
//若出现图形撕裂或者位置显示不正确, 可尝试降低优化等级测试, 也可反馈后续版本更新解决

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

#define _SOFT_3SPI  (0)//软件三线SPI驱动   对应文件stm32f103_lcd_soft_3spi_port.c
#define _SOFT_4SPI  (1)//软件四线SPI驱动   对应文件stm32f103_lcd_soft_4spi_port.c
#define _HARD_4SPI  (2)//硬件四线SPI驱动   对应文件stm32f103_lcd_hard_4spi_port.c
#define _DMA_4SPI   (3)//DMA四线SPI驱动   对应文件stm32f103_lcd_dma_4spi_port.c(暂不支持动态刷新)
#define _SOFT_IIC   (4)//软件IIC驱动(推荐) 对应文件stm32f103_lcd_soft_iic_port.c 
#define _HARD_IIC   (5)//硬件IIC驱动(不推荐) 对应文件stm32f103_lcd_hard_iic_port.c (应答等存在问题暂未解决)
#define LCD_PORT    _HARD_4SPI//选择一个接口




//---------------------------2.1设定屏幕IIC地址--------------------------------

#if ((LCD_PORT == _HARD_IIC) || (LCD_PORT == _SOFT_IIC))
	#define OLED_IIC_7ADDR 0x3C //7位0x3C => 8位0x78 (大部分默认)
	//#define OLED_IIC_7ADDR 0x3D //7位0x3D => 8位0x7A
#endif 

//-------------------------2.2设定硬件SPI时钟速率-------------------------------

#if ((LCD_PORT == _HARD_4SPI) || (LCD_PORT == _DMA_4SPI))
//STM32F103手册指定SPI最高设置18MHz 但也支持超频
#define RCC_HCLK_Divx            RCC_HCLK_Div2 //HCLK时钟分频1,2,4,8,16
#define SPI_BaudRatePrescaler_x  SPI_BaudRatePrescaler_2 //SPI分频2,4,8,16,32,64,128,256
#endif


//---------------------------3.1.选择刷屏方式--------------------------------
#define _FULL_BUFF_FULL_UPDATE (0) //全屏缓存 全屏刷新
#define _FULL_BUFF_DYNA_UPDATE (1) //全屏缓存 动态刷新(更高的刷新速度)(优先使用)
#define _PAGE_BUFF_FULL_UPDATE (2) //页缓存 全屏刷新(更低的内存占用)[暂不支持上位机串流和截屏]
#define _PAGE_BUFF_DYNA_UPDATE (3) //页缓存 动态刷新(更低的内存占用,建议使用)[暂不支持上位机串流和截屏]

#define LCD_MODE    _FULL_BUFF_DYNA_UPDATE //选择一个刷屏模式

//-------------------------3.2.设置择刷屏缓存页大小--------------------------------
//仅页缓存模式需要设置
#if ((LCD_MODE == _PAGE_BUFF_FULL_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
	#define GRAM_YPAGE_NUM (4)//自定义 最小取1 最大取(((SCREEN_HIGH+7)/8))
	//#define GRAM_YPAGE_NUM ((((SCREEN_HIGH+7)/8)+1)/2)//设置二分之一屏缓存
	//#define GRAM_YPAGE_NUM ((((SCREEN_HIGH+7)/8)+3)/4)//设置四分之一屏缓存
	//#define GRAM_YPAGE_NUM ((((SCREEN_HIGH+7)/8)+7)/8)//设置八分之一屏缓存
	//#define GRAM_YPAGE_NUM (((SCREEN_HIGH+7)/8))//设置全屏缓存(请直接使用_FULL_BUFF_xxxx_UPDATE模式获取更高的性能)
#endif



//----------------------------4.设定屏幕分辨率--------------------------------
#define SCREEN_WIDTH 128  //建议取8的倍数
#define SCREEN_HIGH  160  //建议取8的倍数


//-----------------------5.设定屏幕区域显示偏移设置--------------------------
//oled屏幕优先修改驱动初始化
#define SCREEN_X_OFFSET     1 //x左右方向偏移像素
#define SCREEN_Y_OFFSET     2 //y上下方向偏移像素


//-----------------------------6.1选择屏幕IC---------------------------------
#define _SH1106    (0)//普通点阵OLED
#define _SH1108    (1)//普通点阵OLED
#define _SH1107    (2)//普通点阵OLED
#define _SH1115    (3)//普通点阵OLED
#define _SSD1306   (4)//普通点阵OLED (最常用)
#define _SSD1309   (5)//普通点阵OLED 
#define _SSD1312   (6)//普通点阵OLED
#define _SSD1315   (7)//普通点阵OLED
#define _SSD1327   (8)//位灰度OLED
#define _ST7735    (9)//TFT彩屏 RGB565
#define _ST7789VW (10)//TFT彩屏 RGB565
#define _ST7796S  (11)//TFT彩屏 RGB565
#define _GC9A01   (12)//TFT彩屏 RGB565 (未完善)
//#define _ILI9488  (13)//TFT彩屏 RGB111 RGB888 暂不支持

#define LCD_IC _ST7735 //选择一个屏幕IC型号

//---------------------------6.2设定灰度屏--------------------------------
//仅"灰度OLED屏"需要设置

//注意目前驱动限制,屏幕宽高须为8的倍数,否则屏幕可能会有溢出

//1.选择一个灰度屏扫描方向[需要与初始化(A0指令)匹配]
//#define GRAY_DRIVER_0DEG  //方向1
#define GRAY_DRIVER_90DEG //方向2

//2.设置画笔灰度亮度[1:15]
#define GRAY_COLOUR  15 //画笔灰度(亮度)







  
 

//------------编译-----------

#if (LCD_PORT == _SOFT_3SPI)    //软件三线SPI
	#include "stm32f103_lcd_soft_3spi_port.h"
#elif (LCD_PORT == _SOFT_4SPI) //软件四线SPI 
	#include "stm32f103_lcd_soft_4spi_port.h"
#elif (LCD_PORT == _HARD_4SPI) //硬件四线SPI 
	#include "stm32f103_lcd_hard_4spi_port.h"
#elif (LCD_PORT == _DMA_4SPI) //DMA四线SPI 
	#include "stm32f103_lcd_dma_4spi_port.h"
#elif (LCD_PORT == _SOFT_IIC)  //软件IIC   
	#include "stm32f103_lcd_soft_iic_port.h"
#elif (LCD_PORT == _HARD_IIC)  //硬件IIC    
	#include "stm32f103_lcd_hard_iic_port.h"
#elif (LCD_PORT == _DMA_IIC)  //DMA_IIC驱动 (暂不支持)
	//#include "stm32f103_oled_dma_iic_port.h"
	#error ("stm32f103 dma iic driver is not supported!")
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





	
	
#endif
	
