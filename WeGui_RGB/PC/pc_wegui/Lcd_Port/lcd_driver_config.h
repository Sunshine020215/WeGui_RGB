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
  * 版本更新 : WeGui RGB V0.5.3
	* 发布支持GD32E230
	* 新增动态刷新细分功能, 优化大屏动态刷新效果
----------------------------------------------------------------*/



//-------------------------1.选择一个屏幕通讯接口-----------------------------

#define _PC  (0)//PC仿真接口 对应pc_lcd_port.c
#define LCD_PORT    _PC//选择一个接口

//-----------------------------2.选择仿真器---------------------------------
#define _SDL2    (0)//SDL2仿真器
#define LCD_IC _SDL2 //选择一个屏幕仿真器


//----------------------------3.设定屏幕分辨率--------------------------------
#define SCREEN_WIDTH 320  //建议取8的倍数
#define SCREEN_HIGH  172  //建议取8的倍数

//----------------------------4.窗口放大--------------------------------
#define WIN_ZOOM (2)

//-----------------------5.设定屏幕区域显示偏移设置--------------------------
#define SCREEN_X_OFFSET     (0) //x左右方向偏移像素
#define SCREEN_Y_OFFSET     (0) //y上下方向偏移像素

//---------------------------4.选择刷屏方式--------------------------------
#define _FULL_BUFF_FULL_UPDATE (0) //仿真中 仅支持全缓存模式
#define _FULL_BUFF_DYNA_UPDATE (1) //暂不持支仿真
#define _PAGE_BUFF_FULL_UPDATE (2) //暂不持支仿真
#define _PAGE_BUFF_DYNA_UPDATE (3) //暂不持支仿真

#define LCD_MODE    _FULL_BUFF_FULL_UPDATE //选择一个刷屏模式















//------------编译-----------

#if (LCD_PORT == _PC)    //PC仿真
	#include "pc_lcd_port.h"
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

#if (LCD_IC == _SDL2)
	#define LCD_USE_RGB565
	#define LCD_IC_Init() do{}while(0)
	#define LCD_Set_Bright(x) do{}while(0)
	#include "SDL.h"
	#include "TFT_Color.h"
#else
	#warning("No screen ic init!")
#endif

#if (LCD_PORT == _SDL2)
    #define LCD_Port_Init() do{PC_SDL2_Init();}while(0)
#else
    #error("LCD_PORT not support!")
#endif

#define LCD_BL_On() do{}while(0)
#define LCD_BL_Off() do{}while(0)


#endif

