/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#ifndef LCD_WEGUI_CONFIG_H
#define LCD_WEGUI_CONFIG_H



#include "lcd_driver_config.h"


//--------------------------1.选择一个GUI菜单交互方式-----------------------------
//#define WEGUI_USE_NONE_PORT //无交互
//#define WEGUI_USE_6KEY_PORT //6按键交互模式 "上","下","左","右","OK","BACK"
#define WEGUI_USE_4KEY_PORT //4按键交互模式 "上","下","左","右"
//#define WEGUI_USE_2KEY_PORT //(暂未适配)2按键交互模式 "BACK","OK"
//#define WEGUI_USE_EC_PORT //(暂未适配)旋转编码器

/*------------------
 ---默认按键接口--- 
 |OK   | A0  |确定|
 |BACK | C15 |返回|
 |UP   | B13 | 上 |
 |DOWN | B14 | 下 |
 |LEFT | B15 | 左 |
 |RIGHT| B12 | 右 | 
-------------------*/

//--------------------------2.启用UART(上位机功能)-----------------------------
#define WEGUI_UART_ON //启用
//#define WEGUI_UART_OFF //关闭



//仅"TFT彩屏"需要设置
#ifdef LCD_USE_RGB565
//--------------------------3.1彩屏色背光亮度调节-----------------------------
	#define BL_PWM_MAX (8) //背光PWM最大值 单位1ms
	#define BL_PWM_MIN (1) //背光PWM最小值 单位1ms


//--------------------------3.2彩屏色位设置-----------------------------

	//设定色位数量
	//支持1位(0~1,共2色) //同一界面允许同时显示1位色(2种颜色) 0B   1B
	//支持2位(0~3,共4色) //同一界面允许同时显示2位色(4种颜色) 00B  01B  10B  11B
	//支持3位(0~7,共8色) //同一界面允许同时显示3位色(8种颜色) 000B 001B 010B 011B 100B 101B 110B 111B
	#define LCD_COLOUR_BIT (3)  //选择色位
	
	

//--------------------------3.3彩屏菜单主题-----------------------------
	//demo菜单默认颜色 
	//Wegui_mList_Init()会里执行mList_par.theme_colour[n] = COLOUR_MLIST_DEFAULT_n进行初始化
	//程序运行过程也中允许更改mList_par.theme_colour[n]的值从而改变主题颜色
	
	
	#if (LCD_COLOUR_BIT == 1)
	
		//---配色1 白云晴空---
		//#define COLOUR_MLIST_DEFAULT_0  0xffbe //默认菜单背景色0 00  //背景色默认为0
		//#define COLOUR_MLIST_DEFAULT_1  0x4395 //默认菜单画笔色1 01  
		
		//---配色2 大橘为重---
		//#define COLOUR_MLIST_DEFAULT_0  0xfd80 //背景色0 00  //背景色默认为0
		//#define COLOUR_MLIST_DEFAULT_1  0x4a48 //画笔色1 01

		//---配色3 深海蔚蓝---
		#define COLOUR_MLIST_DEFAULT_0  0x530d //背景色0 00  //背景色默认为0
		#define COLOUR_MLIST_DEFAULT_1  0xefbe //画笔色1 01
		
		#define COLOUR_MLIST_NORMAL_TEXT      (1)//菜单列表文字色
		#define COLOUR_MLIST_WSLIDER_NUM      (1)//菜单列表wSlider数字颜色
		#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//菜单列表wMessage字符颜色
		#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//菜单列表wCheckbox外框颜色
		#define COLOUR_MLIST_WCHECKBOX_INTER  (1)//菜单列表wCheckbox内填充颜色
		#define COLOUR_MLIST_SCROOL_BAR       (1)//菜单列表滚动条颜色
		#define COLOUR_MESS_TIP_BOX_BORDER    (1)//wMessage提示弹窗边缘颜色
		#define COLOUR_MESS_TIP_TEXT          (1)//wMessage提示弹窗文字颜色
		#define COLOUR_SLIDER_TIP_BOX_BORDER  (1)//wSlider滑条弹窗边缘颜色
		#define COLOUR_SLIDER_TIP_TEXT        (1)//wSlider滑条弹窗文字颜色
		#define COLOUR_SLIDER_TIP_NUM         (1)//wSlider滑条弹窗数字颜色
		#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider滑条弹窗的滑条边框颜色
		#define COLOUR_SLIDER_TIP_BAR_PROG    (1)//wSlider滑条弹窗的进度颜色
		#define COLOUR_DEBUG_BAR_BG           (0)//调试窗的底色
		#define COLOUR_DEBUG_BAR_BORDER       (1)//调试窗的边框颜色
		#define COLOUR_DEBUG_BAR_TEXT         (1)//调试窗的文字颜色

		
	#elif (LCD_COLOUR_BIT == 2)//同一界面允许同时显示2位色(4种颜色)
	
		//---配色1 白云晴空---
		//#define COLOUR_MLIST_DEFAULT_0  0xffbe //默认菜单背景色0 00  //背景色默认为0
		//#define COLOUR_MLIST_DEFAULT_1  0x4395 //默认菜单画笔色1 01  
		//#define COLOUR_MLIST_DEFAULT_2  0xdf1d //默认菜单画笔色2 10  
		//#define COLOUR_MLIST_DEFAULT_3  0x1169 //默认菜单画笔色3 11  
		//#define COLOUR_MLIST_NORMAL_TEXT      (1)//菜单列表文字色(光标色取反,即2)
		//#define COLOUR_MLIST_WSLIDER_NUM      (1)//菜单列表wSlider数字颜色
		//#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//菜单列表wMessage字符颜色
		//#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//菜单列表wCheckbox外框颜色
		//#define COLOUR_MLIST_WCHECKBOX_INTER  (3)//菜单列表wCheckbox内填充颜色
		//#define COLOUR_MLIST_SCROOL_BAR       (1)//菜单列表滚动条颜色
		//#define COLOUR_MESS_TIP_BOX_BORDER    (1)//wMessage提示弹窗边缘颜色
		//#define COLOUR_MESS_TIP_TEXT          (1)//wMessage提示弹窗文字颜色
		//#define COLOUR_SLIDER_TIP_BOX_BORDER  (1)//wSlider滑条弹窗边缘颜色
		//#define COLOUR_SLIDER_TIP_TEXT        (1)//wSlider滑条弹窗文字颜色
		//#define COLOUR_SLIDER_TIP_NUM         (1)//wSlider滑条弹窗数字颜色
		//#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider滑条弹窗的滑条边框颜色
		//#define COLOUR_SLIDER_TIP_BAR_PROG    (3)//wSlider滑条弹窗的进度颜色
		//#define COLOUR_DEBUG_BAR_BG           (0)//调试窗的底色
		//#define COLOUR_DEBUG_BAR_BORDER       (1)//调试窗的边框颜色
		//#define COLOUR_DEBUG_BAR_TEXT         (1)//调试窗的文字颜色

		//---配色2 大橘为重---
		#define COLOUR_MLIST_DEFAULT_0  0xfd80 //背景色0 00  //背景色默认为0
		#define COLOUR_MLIST_DEFAULT_1  0x4a48 //画笔色1 01
		#define COLOUR_MLIST_DEFAULT_2  0xffdb //画笔色2 10
		#define COLOUR_MLIST_DEFAULT_3  0x3c57 //画笔色3 11
		#define COLOUR_MLIST_NORMAL_TEXT      (1)//菜单列表文字色(光标色取反,即2)
		#define COLOUR_MLIST_WSLIDER_NUM      (1)//菜单列表wSlider数字颜色
		#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//菜单列表wMessage字符颜色
		#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//菜单列表wCheckbox外框颜色
		#define COLOUR_MLIST_WCHECKBOX_INTER  (1)//菜单列表wCheckbox内填充颜色
		#define COLOUR_MLIST_SCROOL_BAR       (1)//菜单列表滚动条颜色
		#define COLOUR_MESS_TIP_BOX_BORDER    (1)//wMessage提示弹窗边缘颜色
		#define COLOUR_MESS_TIP_TEXT          (1)//wMessage提示弹窗文字颜色
		#define COLOUR_SLIDER_TIP_BOX_BORDER  (1)//wSlider滑条弹窗边缘颜色
		#define COLOUR_SLIDER_TIP_TEXT        (1)//wSlider滑条弹窗文字颜色
		#define COLOUR_SLIDER_TIP_NUM         (2)//wSlider滑条弹窗数字颜色
		#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider滑条弹窗的滑条边框颜色
		#define COLOUR_SLIDER_TIP_BAR_PROG    (3)//wSlider滑条弹窗的进度颜色
		#define COLOUR_DEBUG_BAR_BG           (0)//调试窗的底色
		#define COLOUR_DEBUG_BAR_BORDER       (1)//调试窗的边框颜色
		#define COLOUR_DEBUG_BAR_TEXT         (1)//调试窗的文字颜色
		
		
		//---配色3 深海蔚蓝---
		//#define COLOUR_MLIST_DEFAULT_0  0x530d //背景色0 00  //背景色默认为0
		//#define COLOUR_MLIST_DEFAULT_1  0xefbe //画笔色1 01
		//#define COLOUR_MLIST_DEFAULT_2  0xc6bb //画笔色2 10
		//#define COLOUR_MLIST_DEFAULT_3  0x2104 //画笔色3 11
		//#define COLOUR_MLIST_NORMAL_TEXT      (1)//菜单列表文字色(光标色取反,即2)
		//#define COLOUR_MLIST_WSLIDER_NUM      (1)//菜单列表wSlider数字颜色
		//#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//菜单列表wMessage字符颜色
		//#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//菜单列表wCheckbox外框颜色
		//#define COLOUR_MLIST_WCHECKBOX_INTER  (1)//菜单列表wCheckbox内填充颜色
		//#define COLOUR_MLIST_SCROOL_BAR       (1)//菜单列表滚动条颜色
		//#define COLOUR_MESS_TIP_BOX_BORDER    (2)//wMessage提示弹窗边缘颜色
		//#define COLOUR_MESS_TIP_TEXT          (2)//wMessage提示弹窗文字颜色
		//#define COLOUR_SLIDER_TIP_BOX_BORDER  (2)//wSlider滑条弹窗边缘颜色
		//#define COLOUR_SLIDER_TIP_TEXT        (2)//wSlider滑条弹窗文字颜色
		//#define COLOUR_SLIDER_TIP_NUM         (2)//wSlider滑条弹窗数字颜色
		//#define COLOUR_SLIDER_TIP_BAR_BORDER  (2)//wSlider滑条弹窗的滑条边框颜色
		//#define COLOUR_SLIDER_TIP_BAR_PROG    (1)//wSlider滑条弹窗的进度颜色
		//#define COLOUR_DEBUG_BAR_BG           (0)//调试窗的底色
		//#define COLOUR_DEBUG_BAR_BORDER       (1)//调试窗的边框颜色
		//#define COLOUR_DEBUG_BAR_TEXT         (1)//调试窗的文字颜色
		
		//---配色4 血色记忆---
		//#define COLOUR_MLIST_DEFAULT_0  0xd124 //背景色0 00  //背景色默认为0
		//#define COLOUR_MLIST_DEFAULT_1  0x2986 //画笔色1 01  
		//#define COLOUR_MLIST_DEFAULT_2  0xef7d //画笔色2 10  
		//#define COLOUR_MLIST_DEFAULT_3  0x3a09 //画笔色3 11  
		//#define COLOUR_MLIST_NORMAL_TEXT      (1)//菜单列表文字色(光标色取反,即2)
		//#define COLOUR_MLIST_WSLIDER_NUM      (1)//菜单列表wSlider数字颜色
		//#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//菜单列表wMessage字符颜色
		//#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//菜单列表wCheckbox外框颜色
		//#define COLOUR_MLIST_WCHECKBOX_INTER  (3)//菜单列表wCheckbox内填充颜色
		//#define COLOUR_MLIST_SCROOL_BAR       (3)//菜单列表滚动条颜色
		//#define COLOUR_MESS_TIP_BOX_BORDER    (1)//wMessage提示弹窗边缘颜色
		//#define COLOUR_MESS_TIP_TEXT          (1)//wMessage提示弹窗文字颜色
		//#define COLOUR_SLIDER_TIP_BOX_BORDER  (1)//wSlider滑条弹窗边缘颜色
		//#define COLOUR_SLIDER_TIP_TEXT        (1)//wSlider滑条弹窗文字颜色
		//#define COLOUR_SLIDER_TIP_NUM         (2)//wSlider滑条弹窗数字颜色
		//#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider滑条弹窗的滑条边框颜色
		//#define COLOUR_SLIDER_TIP_BAR_PROG    (3)//wSlider滑条弹窗的进度颜色
		//#define COLOUR_DEBUG_BAR_BG           (0)//调试窗的底色
		//#define COLOUR_DEBUG_BAR_BORDER       (1)//调试窗的边框颜色
		//#define COLOUR_DEBUG_BAR_TEXT         (1)//调试窗的文字颜色
		
	#elif (LCD_COLOUR_BIT == 3)//同一界面允许同时显示3位色(8种颜色)
		//---配色2 大橘为重---
		#define COLOUR_MLIST_DEFAULT_0  0xfd80 //背景色0 000  //背景色默认为0
		#define COLOUR_MLIST_DEFAULT_1  0x4a48 //画笔色1 001
		#define COLOUR_MLIST_DEFAULT_2  0xaa05 //画笔色2 010
		#define COLOUR_MLIST_DEFAULT_3  0x3186 //画笔色3 011
		#define COLOUR_MLIST_DEFAULT_4  0xef77 //背景色0 100
		#define COLOUR_MLIST_DEFAULT_5  0x0000 //画笔色1 101
		#define COLOUR_MLIST_DEFAULT_6  0xffdb //画笔色2 110
		#define COLOUR_MLIST_DEFAULT_7  0x3c57 //画笔色3 111
		#define COLOUR_MLIST_NORMAL_TEXT      (1)//菜单列表文字色(光标色取反,即6)
		#define COLOUR_MLIST_WSLIDER_NUM      (1)//菜单列表wSlider数字颜色
		#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//菜单列表wMessage字符颜色
		#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//菜单列表wCheckbox外框颜色
		#define COLOUR_MLIST_WCHECKBOX_INTER  (1)//菜单列表wCheckbox内填充颜色
		#define COLOUR_MLIST_SCROOL_BAR       (1)//菜单列表滚动条颜色
		#define COLOUR_MESS_TIP_BOX_BORDER    (3)//wMessage提示弹窗边缘颜色
		#define COLOUR_MESS_TIP_TEXT          (3)//wMessage提示弹窗文字颜色
		#define COLOUR_SLIDER_TIP_BOX_BORDER  (3)//wSlider滑条弹窗边缘颜色
		#define COLOUR_SLIDER_TIP_TEXT        (3)//wSlider滑条弹窗文字颜色
		#define COLOUR_SLIDER_TIP_NUM         (4)//wSlider滑条弹窗数字颜色
		#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider滑条弹窗的滑条边框颜色
		#define COLOUR_SLIDER_TIP_BAR_PROG    (2)//wSlider滑条弹窗的进度颜色
		#define COLOUR_DEBUG_BAR_BG           (0)//调试窗的底色
		#define COLOUR_DEBUG_BAR_BORDER       (1)//调试窗的边框颜色
		#define COLOUR_DEBUG_BAR_TEXT         (1)//调试窗的文字颜色
	#else
		#error("not support LCD_COLOUR_BIT!")
	#endif
#endif




















//------------编译-----------

//非彩屏,默认使用1位色
#ifndef LCD_USE_RGB565
	#ifdef LCD_COLOUR_BIT
		#undef LCD_COLOUR_BIT
	#endif
	#define LCD_COLOUR_BIT (1)
	#define COLOUR_MLIST_NORMAL_TEXT      (1)//菜单列表文字色
	#define COLOUR_MLIST_WSLIDER_NUM      (1)//菜单列表wSlider数字颜色
	#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//菜单列表wMessage字符颜色
	#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//菜单列表wCheckbox外框颜色
	#define COLOUR_MLIST_WCHECKBOX_INTER  (1)//菜单列表wCheckbox内填充颜色
	#define COLOUR_MLIST_SCROOL_BAR       (1)//菜单列表滚动条颜色
	#define COLOUR_MESS_TIP_BOX_BORDER    (1)//wMessage提示弹窗边缘颜色
	#define COLOUR_MESS_TIP_TEXT          (1)//wMessage提示弹窗文字颜色
	#define COLOUR_SLIDER_TIP_BOX_BORDER  (1)//wSlider滑条弹窗边缘颜色
	#define COLOUR_SLIDER_TIP_TEXT        (1)//wSlider滑条弹窗文字颜色
	#define COLOUR_SLIDER_TIP_NUM         (1)//wSlider滑条弹窗数字颜色
	#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider滑条弹窗的滑条边框颜色
	#define COLOUR_SLIDER_TIP_BAR_PROG    (1)//wSlider滑条弹窗的进度颜色
	#define COLOUR_DEBUG_BAR_BG           (0)//调试窗的底色
	#define COLOUR_DEBUG_BAR_BORDER       (1)//调试窗的边框颜色
	#define COLOUR_DEBUG_BAR_TEXT         (1)//调试窗的文字颜色
#endif


#if defined WEGUI_USE_6KEY_PORT    //6键交互模式
	#include "stm32f103_wegui_6key_port.h"
	#define Wegui_Interface_port_Init() do{Wegui_6key_port_Init();Wegui_Uart_Port_Init();}while(0)
#elif defined WEGUI_USE_4KEY_PORT    //4键交互模式
	#include "stm32f103_wegui_4key_port.h"
	#define Wegui_Interface_port_Init() do{Wegui_4key_port_Init();Wegui_Uart_Port_Init();}while(0)
#elif defined WEGUI_USE_NONE_PORT
	#define Wegui_Interface_port_Init() do{}while(0)
	#define Wegui_Interface_stick(x) do{}while(0)
#endif
		

#if defined WEGUI_UART_ON
	#include "stm32f103_wegui_uart_port.h"
#else //defined Wegui_UART_OFF
	#define Wegui_Uart_Port_Init() do{}while(0) //防止编译错误
	#define Wegui_uart_rx_stick(stick) do{}while(0) //防止编译错误
#endif
	
	
#include "lcd_wegui_menu_app.h"
		
	
#endif
