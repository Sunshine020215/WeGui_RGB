/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#ifndef _USER_Wegui_MENU_H_
#define _USER_Wegui_MENU_H_

#include "lcd_wegui_driver.h"


extern menu_t m_main;
//-------------------m_main主菜单------------------------
extern menu_t m_wDemo;
extern menu_t m_App_ThemeClock;
extern menu_t m_App_UartScreen;
extern menu_t m_App_Setting;
extern menu_t m_App_5;
extern menu_t m_App_6;
extern menu_t m_App_7;
extern menu_t m_App_8;
extern menu_t m_App_9;
extern menu_t m_App_10;



//----------------m.wDemo控件Demo-----------------------
extern menu_t m_wDemo_wMessage_Tip;
extern menu_t m_wDemo_wMessage_ADC;
extern menu_t m_wDemo_wMessage_Pres;
extern menu_t m_wDemo_wMessage_Pres2;
extern menu_t m_wDemo_wCheckbox;
extern menu_t m_wDemo_wSlider1;
extern menu_t m_wDemo_wSlider2;

//-------------------m.Setting设置------------------------
extern menu_t m_Setting_Display;
extern menu_t m_Setting_Time;
extern menu_t m_Setting_Speaker;
extern menu_t m_Setting_UI;
extern menu_t m_Setting_Developer;
extern menu_t m_Setting_Language;
extern menu_t m_Setting_About;

//-------------------m.Setting.Language语言------------------------
extern menu_t m_Setting_Language_English;
extern menu_t m_Setting_Language_Chinese;

//-------------------m.Setting.Display显示------------------------
extern menu_t m_Setting_Display_ScreenFPS;
extern menu_t m_Setting_Display_Brightness;
extern menu_t m_Setting_Display_UI_Speed;

//---------------m.Setting.Developer开发者选项---------------------
extern menu_t m_Setting_Developer_wDebugInfoEn;//调试窗口

//---------------m.Setting.About关于---------------------
extern menu_t m_Setting_About_wCpu;
extern menu_t m_Setting_About_wSoft;


#endif

