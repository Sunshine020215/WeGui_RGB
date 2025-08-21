/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/


#include "main.h"
#include "user_wegui_menu.h"
#include "lcd_wegui_tip.h"
#include "pc_lcd_port.h"

uint32_t sys1ms_stick;
extern uint8_t adc_en;
extern uint32_t apptimer_ms;
extern uint16_t ResADC;

Uint32 sdl_1ms_stick(Uint32 interval, void *param);


void startup_init()
{
    //创建定时器,模拟1ms中断
	SDL_AddTimer(1, sdl_1ms_stick, NULL);

}


//1ms中断函数
Uint32 sdl_1ms_stick(Uint32 interval, void *param)
{
    Wegui_1ms_stick();
    if(sys1ms_stick < 65535)
    {
        sys1ms_stick++;
    }
    return interval;
}


//-----------------------------------------------------
//  !!请自行设置使用UTF8编码格式!!
//  默认使用"方向键"和"WASD"操作控制
//-----------------------------------------------------


//主程序函数
int main( int argc, char * argv[])
{
	startup_init();

	//本框架"Driver"部分为高效率点阵OLED/RGB驱动,可单独移植使用移植
	lcd_driver_Init();

	//本框架Wegui部分为多级菜单图形动画ui,处理多级菜单,过度动画等
	lcd_wegui_init();

	sys1ms_stick = 0;


	while (1)
	{
		//------------1.多级菜单DEMO-------------
		Wegui_loop_func();//Wegui循环驱动

		//--------------主循环-----------------
		if(sys1ms_stick)//1ms动作
		{
			//可做其他功能,例如计时
			apptimer_ms+=sys1ms_stick;
			//模拟ADC
			if(adc_en)
            {
                ResADC++;
                m_wDemo_wMessage_ADC_func();//菜单实时更新ADC值
            }
			sys1ms_stick=0;;
		}


		//------------仿真退出窗口用---------------
		if(sdl_quit!=0){break;}
	}
	return 0;
}

