/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#include "user_wegui_menu.h"


/*************************************************
* 1.需要多国语言显示必须使用UTF8编码
* Edit->Configuration->Encoding->"Encod in UTF8"
* 
* 2.为放置中文字符串出现编译问题,
* 在V5编译器下,需要调整编译规则
* Project -> Oprions for Target-> C/C++ -> 
* Misc Contiols -> 填入"--locale=english"
*************************************************/


//-------------------m主菜单------------------------
menu_t m_main =
{
	.fatherMenu=0x00,//父菜单
	.subMenu=&m_wDemo,//(首个)子菜单
	.nextMenu=0x00,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="主菜单",//中文标题
	  .str_en_US="MAIN",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="",//中文描述
	  .str_en_US="",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};


menu_t m_wDemo =
{
	.fatherMenu=&m_main,//父菜单
	.subMenu=&m_wDemo_wMessage_Tip,//(首个)子菜单
	.nextMenu=&m_App_ThemeClock,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="控件Demo",//中文标题
	  .str_en_US="wDemo",//英文标题
	},                      
	.discribe=              
	{                       
		.str_zh_CN="1.控件Demo",//中文描述
	  .str_en_US="1.Widget demo",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};




void m_App_ThemeClock_Begin(void);//进入APP执行一次
void m_App_ThemeClock_Loop(void);//主循环执行
void m_App_ThemeClock_Quit(void);//退出菜单执行一次
void m_App_ThemeClock_Refresh(uint8_t ui_farmes,uint16_t time_count);//刷新屏幕时执行,放绘图函数
menu_t m_App_ThemeClock =
{
	.fatherMenu=&m_main,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_App_UartScreen,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="主题时钟",//中文标题
	  .str_en_US="CLOCK",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="2.主题时钟",//中文描述
	  .str_en_US="2.Theme Clock",//英文描述
	},
	.menuType=mPorgram,//菜单类型
	.menuPar.mPorgram_Par =
	{
		.begin_fun=m_App_ThemeClock_Begin,//菜单进入 执行一次
		.loop_fun=m_App_ThemeClock_Loop, //菜单功能 持续执行
		.quit_fun=m_App_ThemeClock_Quit, //菜单退出 执行一次
		.refresh_fun=m_App_ThemeClock_Refresh,//刷新屏幕时执行,放绘图函数
	},
};



void Uart_Screen_App_Begin(void);
void Uart_Screen_App_Loop(void);
void Uart_Screen_App_Quit(void);
void Uart_Screen_Refresh(uint8_t ui_farmes,uint16_t time_count);//刷新屏幕时执行,绘图
menu_t m_App_UartScreen =
{
	.fatherMenu=&m_main,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_App_Setting,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="Uart显示器",//中文标题
	  .str_en_US="UART SCREEN",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="3.Uart显示器",//中文描述
	  .str_en_US="3.Uart screen",//英文描述
	},
	.menuType=mPorgram,//菜单类型
	.menuPar.mPorgram_Par=
	{
		.begin_fun=Uart_Screen_App_Begin,//菜单进入 执行一次
		.loop_fun=Uart_Screen_App_Loop, //菜单功能 持续执行
		.quit_fun=Uart_Screen_App_Quit, //菜单退出 执行一次
		.refresh_fun = Uart_Screen_Refresh//刷新屏幕时执行,放绘图函数
	}
};

menu_t m_App_Setting =
{
	.fatherMenu=&m_main,//父菜单
	.subMenu=&m_Setting_Display,//(首个)子菜单
	.nextMenu=&m_App_5,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="设置",//中文标题
	  .str_en_US="SETTING",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="4.设置",//中文描述
	  .str_en_US="4.Setting",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};


menu_t m_App_5 =
{
	.fatherMenu=&m_main,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_App_6,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="应用5",//中文标题
	  .str_en_US="APP 5",//英文标题
	},                      
	.discribe=              
	{                       
		.str_zh_CN="5.应用5",//中文描述
	  .str_en_US="5.App 5",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};
menu_t m_App_6 =
{
	.fatherMenu=&m_main,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_App_7,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="应用6",//中文标题
	  .str_en_US="APP 6",//英文标题
	},                      
	.discribe=              
	{                       
		.str_zh_CN="6.应用6",//中文描述
	  .str_en_US="6.App 6",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};
menu_t m_App_7 =
{
	.fatherMenu=&m_main,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_App_8,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="应用7",//中文标题
	  .str_en_US="APP 7",//英文标题
	},                      
	.discribe=              
	{                       
		.str_zh_CN="7.应用7",//中文描述
	  .str_en_US="7.App 7",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};
menu_t m_App_8 =
{
	.fatherMenu=&m_main,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_App_9,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="应用8",//中文标题
	  .str_en_US="APP 8",//英文标题
	},                      
	.discribe=              
	{                       
		.str_zh_CN="8.应用8",//中文描述
	  .str_en_US="8.App 8",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};
menu_t m_App_9 =
{
	.fatherMenu=&m_main,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_App_10,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="应用9",//中文标题
	  .str_en_US="APP 9",//英文标题
	},                      
	.discribe=              
	{                       
		.str_zh_CN="9.应用9",//中文描述
	  .str_en_US="9.App 9",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};


menu_t m_App_10 =
{
	.fatherMenu=&m_main,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=0x00,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="应用10",//中文标题
	  .str_en_US="APP10",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="10.应用10",//中文描述
	  .str_en_US="10.App 10",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};


//-------------------m.m_Widget_Demo------------------------
menu_t m_wDemo_wMessage_Tip=
{
	.fatherMenu=&m_wDemo,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_wDemo_wCheckbox,//同级下一个菜单
	.titel=
	{
		.str_zh_CN=" ",//中文标题 wMessage可不设置
	  .str_en_US=" ",//英文标题 wMessage可不设置
	},
	.discribe=
	{
		.str_zh_CN="wMessage Tip",//中文描述
	  .str_en_US="wMessage Tip",//英文描述
	},
	.menuType=wMessage,//菜单类型
	.menuPar.wMessage_Par.Tip_string=
	{
		.str_zh_CN="wMessage\n支持换行\n222",//中文标题
	  .str_en_US="wMessage\nnewline A\nnewline B",//英文标题
	},
};


//-------------------m.m_wDemo_wMessage------------------------

extern uint8_t adc_en;
menu_t m_wDemo_wCheckbox=
{
	.fatherMenu=&m_wDemo,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_wDemo_wMessage_ADC,//同级下一个菜单
	.titel=
	{
		.str_zh_CN=" ",//中文标题 wMessage可不设置
	  .str_en_US=" ",//英文标题 wMessage可不设置
	},
	.discribe=
	{
		.str_zh_CN="wCheckBox",//中文描述
	  .str_en_US="wCheckBox",//英文描述
	},
	.menuType=wCheckBox,//菜单类型
	.menuPar.wCheckBox_Par =
	{
		.pstr=&adc_en,         //复选框控制的数据指针
		.Change_Value=0x00,//值被修改 执行一次
	},
};

char m_wDemo_wMessage_ADC_Value_string[7];
menu_t m_wDemo_wMessage_ADC=
{
	.fatherMenu=&m_wDemo,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_wDemo_wMessage_Pres,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="",//中文标题  wMessage可不设置
	  .str_en_US="",//英文标题  wMessage可不设置
	},
	.discribe=
	{
		.str_zh_CN="wMess ADC",//中文描述 
	  .str_en_US="wMess ADC",//英文描述
	},
	.menuType=wMessage,//菜单类型
	.menuPar.wMessage_Par=
	{
		.Press_func=0x00,
		.Value_string=m_wDemo_wMessage_ADC_Value_string,
		.Tip_string=
		{
			.str_zh_CN="可调电阻ADC\nIO: A1",//中文提示框
			.str_en_US="ResADC\nIO: A1",//英文提示框
		},
	},
};

char wMessage_Pres_Par_Value_string[7];
void m_wDemo_wMessage_Pres_func(void);
menu_t m_wDemo_wMessage_Pres=
{
	.fatherMenu=&m_wDemo,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_wDemo_wMessage_Pres2,//同级下一个菜单
	.titel=
	{
		.str_zh_CN=" ",//中文标题 wMessage可不设置
	  .str_en_US=" ",//英文标题 wMessage可不设置
	},
	.discribe=
	{
		.str_zh_CN="wMes 按下",//中文描述
	  .str_en_US="wMes pres",//英文描述
	},
	.menuType=wMessage,//菜单类型
	.menuPar.wMessage_Par=
	{
		.Press_func=m_wDemo_wMessage_Pres_func,
		.Value_string=wMessage_Pres_Par_Value_string,
		.Tip_string=
		{
			.str_zh_CN="",//中文提示框
			.str_en_US="",//英文提示框
		},
	},
};


char wMessage_Pres2_Par_Value_string[3] = "--";
void m_wDemo_wMessage_Pres2_func(void);
menu_t m_wDemo_wMessage_Pres2=
{
	.fatherMenu=&m_wDemo,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_wDemo_wSlider1,//同级下一个菜单
	.titel=
	{
		.str_zh_CN=" ",//中文标题 wMessage可不设置
	  .str_en_US=" ",//英文标题 wMessage可不设置
	},
	.discribe=
	{
		.str_zh_CN="wMes 按下",//中文描述
	  .str_en_US="wMes pres",//英文描述
	},
	.menuType=wMessage,//菜单类型
	.menuPar.wMessage_Par=
	{
		.Press_func=m_wDemo_wMessage_Pres2_func,
		.Value_string=wMessage_Pres2_Par_Value_string,
		.Tip_string=
		{
			.str_zh_CN="",//中文提示框
			.str_en_US="",//英文提示框
		},
	},
};




int16_t slider_demo_value=-5;
menu_t m_wDemo_wSlider1 =
{
	.fatherMenu=&m_wDemo,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_wDemo_wSlider2,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="demo1",//中文标题 弹窗里显示的名称
	  .str_en_US="demo1",//英文标题 弹窗里显示的名称
	},
	.discribe=
	{
		.str_zh_CN="wSlider",//中文描述 列表里显示的名称
	  .str_en_US="wSlider",//英文描述 列表里显示的名称
	},
	.menuType=wSlider,//菜单类型
	.menuPar.wSliderTip_Par =
	{
		.Push_tip_func=0x00,      //控件进入 执行一次
		.Change_Value_func=0x00,  //值被修改 执行一次
		.End_tip_func=0x00,       //控件退出 执行一次
		
		.pstr = &slider_demo_value,
		.min = -100,
		.max = 100,
	},
};

menu_t m_wDemo_wSlider2 =
{
	.fatherMenu=&m_wDemo,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=0x00,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="---demo2----\nnewline\nnewline\nnewline",//中文标题 弹窗里显示的名称
	  .str_en_US="---demo2----\nnewline\nnewline\nnewline",//英文标题 弹窗里显示的名称
	},
	.discribe=
	{
		.str_zh_CN="wSlider",//中文描述 列表里显示的名称
	  .str_en_US="wSlider",//英文描述 列表里显示的名称
	},
	.menuType=wSlider,//菜单类型
	.menuPar.wSliderTip_Par =
	{
		.Push_tip_func=0x00,    //控件进入 执行一次
		.Change_Value_func=0x00,//值被修改 执行一次
		.End_tip_func=0x00,     //控件退出 执行一次
		
		.pstr = &slider_demo_value,
		.min = -100,
		.max = 100,
	},
};



//-------------------m.m_App_Setting------------------------
menu_t m_Setting_Display =
{
	.fatherMenu=&m_App_Setting,//父菜单
	.subMenu=&m_Setting_Display_Brightness,//(首个)子菜单
	.nextMenu=&m_Setting_Time,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="显示",//中文标题
	  .str_en_US="DISPLAY",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="1.显示",//中文描述
	  .str_en_US="1.Display",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};

menu_t m_Setting_Time =
{
	.fatherMenu=&m_App_Setting,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_Setting_Speaker,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="时间",//中文标题
	  .str_en_US="Time&Clock",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="2.时间",//中文描述
	  .str_en_US="2.Time&Clock",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};

menu_t m_Setting_Speaker =
{
	.fatherMenu=&m_App_Setting,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_Setting_UI,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="音频",//中文标题
	  .str_en_US="SPEAKER",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="3.音频",//中文描述
	  .str_en_US="3.Speaker",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};


menu_t m_Setting_UI =
{
	.fatherMenu=&m_App_Setting,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_Setting_Language,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="UI&主题",//中文标题
	  .str_en_US="UI THEME",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="5.UI&主题",//中文描述
	  .str_en_US="5.UI Theme",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};

menu_t m_Setting_Language =
{
	.fatherMenu=&m_App_Setting,//父菜单
	.subMenu=&m_Setting_Language_English,//(首个)子菜单
	.nextMenu=&m_Setting_Developer,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="系统:简体中文 ",//中文标题
	  .str_en_US="SYS:English",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="6.语言",//中文描述
	  .str_en_US="6.Language",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};

menu_t m_Setting_Developer =
{
	.fatherMenu=&m_App_Setting,//父菜单
	.subMenu=&m_Setting_Developer_wDebugInfoEn,//(首个)子菜单
	.nextMenu=&m_Setting_About,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="开发者选项",//中文标题
	  .str_en_US="DEVELOPER",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="7.开发者选项",//中文描述
	  .str_en_US="7.Developer",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};



menu_t m_Setting_About=
{
	.fatherMenu=&m_App_Setting,//父菜单
	.subMenu=&m_Setting_About_wCpu,//(首个)子菜单
	.nextMenu=0x00,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="关于",//中文标题
	  .str_en_US="ABOUT",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="8.关于",//中文描述
	  .str_en_US="8.About",//英文描述
	},
	.menuType=mList,//菜单类型
	.menuPar.mList_Par =
	{
		.begin_fun=0x00,//菜单进入 执行一次
		.loop_fun=0x00, //菜单功能 持续执行
		.quit_fun=0x00, //菜单退出 执行一次
	},
};


//-------------------m.Setting.Language语言------------------------
void Set_language_English(void);
menu_t m_Setting_Language_English =
{
	.fatherMenu=&m_Setting_Language,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_Setting_Language_Chinese,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="ENGLISH",//中文标题
	  .str_en_US="ENGLISH",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="1.English",//中文描述
	  .str_en_US="1.English",//英文描述
	},

	.menuType=wMessage,//菜单类型
	.menuPar.wMessage_Par =
	{
		.Press_func=Set_language_English,//按下执行一次
		.Value_string=0x00,              //末尾显示的值 0x00不显示
		.Tip_string=
		{
			.str_zh_CN="error",//中文提示框
			.str_en_US="English now!",//英文提示框
		}, 
	},
};

void Set_language_Chinese(void);
menu_t m_Setting_Language_Chinese =
{
	.fatherMenu=&m_Setting_Language,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=0x00,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="简体中文",//中文标题
	  .str_en_US="简体中文",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="2.简体中文",//中文描述
	  .str_en_US="2.简体中文",//英文描述
	},
	.menuType=wMessage,//菜单类型
	.menuPar.wMessage_Par =
	{
		.Press_func=Set_language_Chinese, //按下执行一次
		.Value_string=0x00,               //末尾显示的值 0x00不显示
		.Tip_string=
		{
			.str_zh_CN="已设置简体中文",//中文提示框
			.str_en_US="error",//英文提示框
		}, 
	},
	
	
};
//-------------------m.Setting.Display显示------------------------
void update_Wegui_screen_brightness(void);
menu_t m_Setting_Display_Brightness =
{
	.fatherMenu=&m_Setting_Display,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_Setting_Display_ScreenFPS,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="对比度",//中文标题
	  .str_en_US="Bright",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="1.对比度",//中文描述
	  .str_en_US="1.Bright",//英文描述
	},
	.menuType=wSlider,//菜单类型
	.menuPar.wSliderTip_Par =
	{
		.Push_tip_func=0x00,                                //控件进入 执行一次
		.Change_Value_func=update_Wegui_screen_brightness,  //值被修改 执行一次
		.End_tip_func=0x00,                                 //控件退出 执行一次
		.pstr = &Wegui.setting.brightness,
		
		#if (defined LCD_USE_RGB565)
		.min = BL_PWM_MIN,
		.max = BL_PWM_MAX,
		#else
		.min = 0,
		.max = 255,
		#endif

	},
};


menu_t m_Setting_Display_ScreenFPS =
{
	.fatherMenu=&m_Setting_Display,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_Setting_Display_UI_Speed,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="屏幕刷新时间\nms",//中文标题
	  .str_en_US="Screen ms",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="2.屏幕帧率",//中文描述
	  .str_en_US="2.Screen FPS",//英文描述
	},
	.menuType=wSlider,//菜单类型
	.menuPar.wSliderTip_Par =
	{
		.Push_tip_func=0x00,      //控件进入 执行一次
		.Change_Value_func=0x00,  //值被修改 执行一次
		.End_tip_func=0x00,       //控件退出 执行一次
		.pstr = &Wegui.setting.screen_fps_ms,
		.min = 1,
		.max = 100,
	},
};


menu_t m_Setting_Display_UI_Speed =
{
	.fatherMenu=&m_Setting_Display,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=0x00,//同级下一个菜单
	.titel=
	{
		.str_zh_CN="UI刷新时间\nms",//中文标题
	  .str_en_US="UI ms",//英文标题
	},
	.discribe=
	{
		.str_zh_CN="3.动画帧率 ",//中文描述
	  .str_en_US="3.UI FPS ",//英文描述
	},
	.menuType=wSlider,//菜单类型
	.menuPar.wSliderTip_Par =
	{
		.Push_tip_func=0x00,      //控件进入 执行一次
		.Change_Value_func=0x00,  //值被修改 执行一次
		.End_tip_func=0x00,       //控件退出 执行一次

		//.pstr_type = W_INT16,
		//.pstr_type = W_FLOAT,
		.pstr = &Wegui.setting.ui_fps_ms,
		.min = 1,
		.max = 100,
	},
};



//---------------m.Setting.Developer开发者选项---------------------
menu_t m_Setting_Developer_wDebugInfoEn=
{
	.fatherMenu=&m_Setting_Developer,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=0x00,//同级下一个菜单
	.titel=
	{
		.str_zh_CN=" ",//中文标题 控件可不设置标题
	  .str_en_US=" ",//英文标题 控件可不设置标题
	},
	.discribe=
	{
		.str_zh_CN="1.调试窗口",//中文描述
	  .str_en_US="1.Debug Windows",//英文描述
	},
	.menuType=wCheckBox,//菜单类型
	.menuPar.wCheckBox_Par =
	{
		.pstr=&Wegui.setting.debug_windows_en,//复选框控制的数据指针
		.Change_Value=0x00,//值被修改 执行一次
	},
};

//---------------m_Setting_About开发者选项---------------------
menu_t m_Setting_About_wCpu=
{
	.fatherMenu=&m_Setting_About,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=&m_Setting_About_wSoft,//同级下一个菜单
	.titel=
	{
		.str_zh_CN=" ",//中文标题 wMessage可不设置
	  .str_en_US=" ",//英文标题 wMessage可不设置
	},
	.discribe=
	{
		.str_zh_CN="主控",//中文描述
	  .str_en_US="MCU",//英文描述
	},
	.menuType=wMessage,//菜单类型
	.menuPar.wMessage_Par.Value_string = STR_MCU_CLASS,
	.menuPar.wMessage_Par.Tip_string=
	{
		.str_zh_CN=STR_MCU_MODEL,//中文标题
	  .str_en_US=STR_MCU_MODEL,//英文标题
	},
};

menu_t m_Setting_About_wSoft=
{
	.fatherMenu=&m_Setting_About,//父菜单
	.subMenu=0x00,//(首个)子菜单
	.nextMenu=0x00,//同级下一个菜单
	.titel=
	{
		.str_zh_CN=" ",//中文标题 wMessage可不设置
	  .str_en_US=" ",//英文标题 wMessage可不设置
	},
	.discribe=
	{
		.str_zh_CN="Software",//中文描述
	  .str_en_US="Software",//英文描述
	},
	.menuType=wMessage,//菜单类型
	.menuPar.wMessage_Par.Value_string = STR_WEGUI_VERSION_CLASS,
	.menuPar.wMessage_Par.Tip_string =
	{
<<<<<<< HEAD:WeGui_RGB/Lcd_WeGui/user_wegui_menu.c
		.str_zh_CN=STR_WEGUI_VERSION,//中文标题
	  .str_en_US=STR_WEGUI_VERSION,//英文标题
=======
		.str_zh_CN=(uint8_t *)"WeGui RGB\n V0.5.3",//中文标题
	  .str_en_US=(uint8_t *)"WeGui RGB\n V0.5.3",//英文标题
>>>>>>> 1d7ada77862ed6167982759a7e115ba62360d19b:Lcd_WeGui/user_wegui_menu.c
	},
};


