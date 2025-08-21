/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#include "lcd_wegui_menu_app.h"

#include "lcd_wegui_driver.h"
#include "user_wegui_menu.h"


//------------------------------------------主题时钟APP DEMO--------------------------------------------
//按键功能移步____key_port.c
extern uint32_t apptimer_ms;
void m_App_ThemeClock_Begin()//进入APP执行一次
{
	Lcd_Clear_GRAM();
}
void m_App_ThemeClock_Loop()//相对于主循环执行
{

}
void m_App_ThemeClock_Quit()//退出菜单执行一次
{

}

uint32_t apptimer_ms=0;
void m_App_ThemeClock_Refresh(uint8_t ui_farmes,uint16_t time_count)//刷新屏幕时执行,放绘图函数
{
	//演示demo,执行效率不佳
	uint16_t i,hour,minute,sec,ms;
	char string[12];

	i = apptimer_ms;
	hour = i / 3600000;
	i %= 3600000;
	minute = i / 60000;
	i %= 60000;
	sec = i / 1000;
	i %= 1000;
	ms = i;

	string[0]=hour/10 + 0x30;//ASCII中0的位置是0x30
	string[1]=hour%10 + 0x30;

	string[3]=minute/10 + 0x30;
	string[4]=minute%10 + 0x30;

	string[6]=sec/10 + 0x30;
	string[7]=sec%10 + 0x30;

	string[8]=0;


	if(ms > 500)
	{
		string[2]=' ';
		string[5]=' ';
	}
	else
	{
		string[2]=':';
		string[5]=':';
	}


	{
		//绘图 演示
		Lcd_Clear_GRAM();
		Lcd_Set_Driver_Mode(write_1);//选择笔刷颜色1
		Lcd_Draw_UTF8_String(0,0,string);
	}
}

//------------------------------------------串口显示器功能菜单--------------------------------------------

void Uart_Screen_App_Begin()//进入APP执行一次
{
	const Wegui_string_t welcom =
	{
		.str_zh_CN="等待上位机操作..",
		.str_en_US="Wait data...",
	};

	Lcd_Clear_GRAM();
	Lcd_Draw_UTF8_String(0,0,Wegui_get_string(welcom,Wegui.setting.language));
}
void Uart_Screen_App_Loop()//相对于主循环执行
{

}
void Uart_Screen_App_Quit()//退出菜单执行一次
{

}

void Uart_Screen_Refresh(uint8_t ui_farmes,uint16_t time_count)//刷新屏幕时执行,放绘图函数
{

}




//------------------------------------------控件演示Demo--------------------------------------------
uint16_t ResADC;
uint8_t adc_en=1;
extern char wMessage_Pres_Par_Value_string[7];
extern char m_wDemo_wMessage_ADC_Value_string[7];
void m_wDemo_wMessage_ADC_func()//菜单实时更新ADC值
{
	my_itoa(ResADC,m_wDemo_wMessage_ADC_Value_string,10);//ResADC转成10进制带符号的字符串存进wMessage_Pres_Par_Value_string
}
void m_wDemo_wMessage_Pres_func()//菜单按下弹窗
{
	my_itoa(ResADC,wMessage_Pres_Par_Value_string,10);//ResADC转成10进制带符号的字符串存进wMessage_Pres_Par_Value_string
}

extern char wMessage_Par_Value_string[3];
void m_wDemo_wMessage_Pres2_func()//菜单按下更换文字
{
	static uint8_t num=0;

	if(num < 2)
	{
		num++;
	}
	else
	{
		num=0;
	}

	switch(num)
	{
		case 0:
			m_wDemo_wMessage_Pres2.menuPar.wMessage_Par.Value_string[0]='A';
			m_wDemo_wMessage_Pres2.menuPar.wMessage_Par.Value_string[1]='a';
		break;
		case 1:
			m_wDemo_wMessage_Pres2.menuPar.wMessage_Par.Value_string[0]='B';
			m_wDemo_wMessage_Pres2.menuPar.wMessage_Par.Value_string[1]='b';
		break;
		case 2:
			m_wDemo_wMessage_Pres2.menuPar.wMessage_Par.Value_string[0]='C';
			m_wDemo_wMessage_Pres2.menuPar.wMessage_Par.Value_string[1]='c';
		break;
		case 3:
			m_wDemo_wMessage_Pres2.menuPar.wMessage_Par.Value_string[0]='D';
			m_wDemo_wMessage_Pres2.menuPar.wMessage_Par.Value_string[1]='d';
		break;
	}
}


//------------------------------------------设置亮度--------------------------------------------
#if (defined LCD_USE_RGB565)
extern uint8_t bl_pwmd;
void update_Wegui_screen_brightness()
{
	bl_pwmd = Wegui.setting.brightness;
}
#else
void update_Wegui_screen_brightness()
{
	LCD_Set_Bright(Wegui.setting.brightness);
}
#endif

//------------------------------------------设置语言--------------------------------------------
void Set_language_Chinese()
{
	Wegui.setting.language=zh_CN;
}
void Set_language_English()
{
	Wegui.setting.language=en_US;
}

