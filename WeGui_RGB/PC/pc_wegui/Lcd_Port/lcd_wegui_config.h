/*
	Copyright 2025 Lu Zhihao
	���������ѧϰ��;, �ݲ�������������;����Ȩ
*/

#ifndef LCD_WEGUI_CONFIG_H
#define LCD_WEGUI_CONFIG_H



#include "lcd_driver_config.h"


#define STR_MODEL_CLASS          "Windows"
#define STR_MODEL                "Windows 11"
#define STR_MCU_CLASS            "Intel"
#define STR_MCU_MODEL            "Core(TM) i9-11900KB"
#define STR_WEGUI_VERSION_CLASS  "V0.5"
#define STR_WEGUI_VERSION        "WeGui RGB\n V0.5.3"


//--------------------------1.ѡ��һ���˵�--------------------

#define _MENU_DEMO    (0) //ѡ����ʾ�˵�
#define _MENU_PROJECT (1) //��Ŀ�Զ���˵�
#define WEGUI_MENU_SELECT _MENU_PROJECT

#define MENU_DEEP 3//���ò˵���������(���ڷ������,��������ʷλ��,ȡС�˷��ز˵�ʱ���ܲ���ص���һ�ε�λ��)

//--------------------------2.ѡ��һ��GUI�˵�������ʽ-----------------------------
#define _DEMO_NONE_PORT     (0) //�޽�������
#define _DEMO_4KEY_PORT     (1) //4���������ӿ� "��","��","��","��"
#define WEGUI_INTERFACE_PORT  _DEMO_4KEY_PORT //ѡ��һ�������ӿ�



//��"TFT����"��Ҫ����
#ifdef LCD_USE_RGB565
//--------------------------3.1����ɫ�������ȵ���-----------------------------
	#define BL_PWM_MAX (4) //����PWM���ֵ ��λ1ms
	#define BL_PWM_MIN (1) //����PWM��Сֵ ��λ1ms


//--------------------------3.2����ɫλ����-----------------------------

	//�趨ɫλ����
	//֧��1λ(0~1,��2ɫ) //ͬһ��������ͬʱ��ʾ1λɫ(2����ɫ) 0B   1B
	//֧��2λ(0~3,��4ɫ) //ͬһ��������ͬʱ��ʾ2λɫ(4����ɫ) 00B  01B  10B  11B
	//֧��3λ(0~7,��8ɫ) //ͬһ��������ͬʱ��ʾ3λɫ(8����ɫ) 000B 001B 010B 011B 100B 101B 110B 111B
	#define LCD_COLOUR_BIT (2)  //ѡ��ɫλ



//--------------------------3.3�����˵�����-----------------------------
	//demo�˵�Ĭ����ɫ
	//Wegui_mList_Init()����ִ��mList_par.theme_colour[n] = COLOUR_MLIST_DEFAULT_n���г�ʼ��
	//�������й���Ҳ���������mList_par.theme_colour[n]��ֵ�Ӷ��ı�������ɫ


	#if (LCD_COLOUR_BIT == 1)

		//---��ɫ1 �������---
		//#define COLOUR_MLIST_DEFAULT_0  0xffbe //Ĭ�ϲ˵�����ɫ0 00  //����ɫĬ��Ϊ0
		//#define COLOUR_MLIST_DEFAULT_1  0x4395 //Ĭ�ϲ˵�����ɫ1 01

		//---��ɫ2 ����Ϊ��---
		//#define COLOUR_MLIST_DEFAULT_0  0xfd80 //����ɫ0 00  //����ɫĬ��Ϊ0
		//#define COLOUR_MLIST_DEFAULT_1  0x4a48 //����ɫ1 01

		//---��ɫ3 �ε��---
		#define COLOUR_MLIST_DEFAULT_0  0x530d //����ɫ0 00  //����ɫĬ��Ϊ0
		#define COLOUR_MLIST_DEFAULT_1  0xefbe //����ɫ1 01

		#define COLOUR_MLIST_NORMAL_TEXT      (1)//�˵��б�����ɫ
		#define COLOUR_MLIST_WSLIDER_NUM      (1)//�˵��б�wSlider������ɫ
		#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//�˵��б�wMessage�ַ���ɫ
		#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//�˵��б�wCheckbox�����ɫ
		#define COLOUR_MLIST_WCHECKBOX_INTER  (1)//�˵��б�wCheckbox�������ɫ
		#define COLOUR_MLIST_SCROOL_BAR       (1)//�˵��б��������ɫ
		#define COLOUR_MESS_TIP_BOX_BORDER    (1)//wMessage��ʾ������Ե��ɫ
		#define COLOUR_MESS_TIP_TEXT          (1)//wMessage��ʾ����������ɫ
		#define COLOUR_SLIDER_TIP_BOX_BORDER  (1)//wSlider����������Ե��ɫ
		#define COLOUR_SLIDER_TIP_TEXT        (1)//wSlider��������������ɫ
		#define COLOUR_SLIDER_TIP_NUM         (1)//wSlider��������������ɫ
		#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider���������Ļ����߿���ɫ
		#define COLOUR_SLIDER_TIP_BAR_PROG    (1)//wSlider���������Ľ�����ɫ
		#define COLOUR_DEBUG_BAR_BG           (0)//���Դ��ĵ�ɫ
		#define COLOUR_DEBUG_BAR_BORDER       (1)//���Դ��ı߿���ɫ
		#define COLOUR_DEBUG_BAR_TEXT         (1)//���Դ���������ɫ


	#elif (LCD_COLOUR_BIT == 2)//ͬһ��������ͬʱ��ʾ2λɫ(4����ɫ)

		//---��ɫ1 �������---
		//#define COLOUR_MLIST_DEFAULT_0  0xffbe //Ĭ�ϲ˵�����ɫ0 00  //����ɫĬ��Ϊ0
		//#define COLOUR_MLIST_DEFAULT_1  0x4395 //Ĭ�ϲ˵�����ɫ1 01
		//#define COLOUR_MLIST_DEFAULT_2  0xdf1d //Ĭ�ϲ˵�����ɫ2 10
		//#define COLOUR_MLIST_DEFAULT_3  0x1169 //Ĭ�ϲ˵�����ɫ3 11
		//#define COLOUR_MLIST_NORMAL_TEXT      (1)//�˵��б�����ɫ(���ɫȡ��,��2)
		//#define COLOUR_MLIST_WSLIDER_NUM      (1)//�˵��б�wSlider������ɫ
		//#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//�˵��б�wMessage�ַ���ɫ
		//#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//�˵��б�wCheckbox�����ɫ
		//#define COLOUR_MLIST_WCHECKBOX_INTER  (3)//�˵��б�wCheckbox�������ɫ
		//#define COLOUR_MLIST_SCROOL_BAR       (1)//�˵��б��������ɫ
		//#define COLOUR_MESS_TIP_BOX_BORDER    (1)//wMessage��ʾ������Ե��ɫ
		//#define COLOUR_MESS_TIP_TEXT          (1)//wMessage��ʾ����������ɫ
		//#define COLOUR_SLIDER_TIP_BOX_BORDER  (1)//wSlider����������Ե��ɫ
		//#define COLOUR_SLIDER_TIP_TEXT        (1)//wSlider��������������ɫ
		//#define COLOUR_SLIDER_TIP_NUM         (1)//wSlider��������������ɫ
		//#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider���������Ļ����߿���ɫ
		//#define COLOUR_SLIDER_TIP_BAR_PROG    (3)//wSlider���������Ľ�����ɫ
		//#define COLOUR_DEBUG_BAR_BG           (0)//���Դ��ĵ�ɫ
		//#define COLOUR_DEBUG_BAR_BORDER       (1)//���Դ��ı߿���ɫ
		//#define COLOUR_DEBUG_BAR_TEXT         (1)//���Դ���������ɫ

		//---��ɫ2 ����Ϊ��---
		#define COLOUR_MLIST_DEFAULT_0  0xfd80 //����ɫ0 00  //����ɫĬ��Ϊ0
		#define COLOUR_MLIST_DEFAULT_1  0x4a48 //����ɫ1 01
		#define COLOUR_MLIST_DEFAULT_2  0xffdb //����ɫ2 10
		#define COLOUR_MLIST_DEFAULT_3  0x3c57 //����ɫ3 11
		#define COLOUR_MLIST_NORMAL_TEXT      (1)//�˵��б�����ɫ(���ɫȡ��,��2)
		#define COLOUR_MLIST_WSLIDER_NUM      (1)//�˵��б�wSlider������ɫ
		#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//�˵��б�wMessage�ַ���ɫ
		#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//�˵��б�wCheckbox�����ɫ
		#define COLOUR_MLIST_WCHECKBOX_INTER  (1)//�˵��б�wCheckbox�������ɫ
		#define COLOUR_MLIST_SCROOL_BAR       (1)//�˵��б��������ɫ
		#define COLOUR_MESS_TIP_BOX_BORDER    (1)//wMessage��ʾ������Ե��ɫ
		#define COLOUR_MESS_TIP_TEXT          (1)//wMessage��ʾ����������ɫ
		#define COLOUR_SLIDER_TIP_BOX_BORDER  (1)//wSlider����������Ե��ɫ
		#define COLOUR_SLIDER_TIP_TEXT        (1)//wSlider��������������ɫ
		#define COLOUR_SLIDER_TIP_NUM         (2)//wSlider��������������ɫ
		#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider���������Ļ����߿���ɫ
		#define COLOUR_SLIDER_TIP_BAR_PROG    (3)//wSlider���������Ľ�����ɫ
		#define COLOUR_DEBUG_BAR_BG           (0)//���Դ��ĵ�ɫ
		#define COLOUR_DEBUG_BAR_BORDER       (1)//���Դ��ı߿���ɫ
		#define COLOUR_DEBUG_BAR_TEXT         (1)//���Դ���������ɫ


		//---��ɫ3 �ε��---
		//#define COLOUR_MLIST_DEFAULT_0  0x530d //����ɫ0 00  //����ɫĬ��Ϊ0
		//#define COLOUR_MLIST_DEFAULT_1  0xefbe //����ɫ1 01
		//#define COLOUR_MLIST_DEFAULT_2  0xc6bb //����ɫ2 10
		//#define COLOUR_MLIST_DEFAULT_3  0x2104 //����ɫ3 11
		//#define COLOUR_MLIST_NORMAL_TEXT      (1)//�˵��б�����ɫ(���ɫȡ��,��2)
		//#define COLOUR_MLIST_WSLIDER_NUM      (1)//�˵��б�wSlider������ɫ
		//#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//�˵��б�wMessage�ַ���ɫ
		//#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//�˵��б�wCheckbox�����ɫ
		//#define COLOUR_MLIST_WCHECKBOX_INTER  (1)//�˵��б�wCheckbox�������ɫ
		//#define COLOUR_MLIST_SCROOL_BAR       (1)//�˵��б��������ɫ
		//#define COLOUR_MESS_TIP_BOX_BORDER    (2)//wMessage��ʾ������Ե��ɫ
		//#define COLOUR_MESS_TIP_TEXT          (2)//wMessage��ʾ����������ɫ
		//#define COLOUR_SLIDER_TIP_BOX_BORDER  (2)//wSlider����������Ե��ɫ
		//#define COLOUR_SLIDER_TIP_TEXT        (2)//wSlider��������������ɫ
		//#define COLOUR_SLIDER_TIP_NUM         (2)//wSlider��������������ɫ
		//#define COLOUR_SLIDER_TIP_BAR_BORDER  (2)//wSlider���������Ļ����߿���ɫ
		//#define COLOUR_SLIDER_TIP_BAR_PROG    (1)//wSlider���������Ľ�����ɫ
		//#define COLOUR_DEBUG_BAR_BG           (0)//���Դ��ĵ�ɫ
		//#define COLOUR_DEBUG_BAR_BORDER       (1)//���Դ��ı߿���ɫ
		//#define COLOUR_DEBUG_BAR_TEXT         (1)//���Դ���������ɫ

		//---��ɫ4 Ѫɫ����---
		//#define COLOUR_MLIST_DEFAULT_0  0xd124 //����ɫ0 00  //����ɫĬ��Ϊ0
		//#define COLOUR_MLIST_DEFAULT_1  0x2986 //����ɫ1 01
		//#define COLOUR_MLIST_DEFAULT_2  0xef7d //����ɫ2 10
		//#define COLOUR_MLIST_DEFAULT_3  0x3a09 //����ɫ3 11
		//#define COLOUR_MLIST_NORMAL_TEXT      (1)//�˵��б�����ɫ(���ɫȡ��,��2)
		//#define COLOUR_MLIST_WSLIDER_NUM      (1)//�˵��б�wSlider������ɫ
		//#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//�˵��б�wMessage�ַ���ɫ
		//#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//�˵��б�wCheckbox�����ɫ
		//#define COLOUR_MLIST_WCHECKBOX_INTER  (3)//�˵��б�wCheckbox�������ɫ
		//#define COLOUR_MLIST_SCROOL_BAR       (3)//�˵��б��������ɫ
		//#define COLOUR_MESS_TIP_BOX_BORDER    (1)//wMessage��ʾ������Ե��ɫ
		//#define COLOUR_MESS_TIP_TEXT          (1)//wMessage��ʾ����������ɫ
		//#define COLOUR_SLIDER_TIP_BOX_BORDER  (1)//wSlider����������Ե��ɫ
		//#define COLOUR_SLIDER_TIP_TEXT        (1)//wSlider��������������ɫ
		//#define COLOUR_SLIDER_TIP_NUM         (2)//wSlider��������������ɫ
		//#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider���������Ļ����߿���ɫ
		//#define COLOUR_SLIDER_TIP_BAR_PROG    (3)//wSlider���������Ľ�����ɫ
		//#define COLOUR_DEBUG_BAR_BG           (0)//���Դ��ĵ�ɫ
		//#define COLOUR_DEBUG_BAR_BORDER       (1)//���Դ��ı߿���ɫ
		//#define COLOUR_DEBUG_BAR_TEXT         (1)//���Դ���������ɫ

	#elif (LCD_COLOUR_BIT == 3)//ͬһ��������ͬʱ��ʾ3λɫ(8����ɫ)
		//---��ɫ2 ����Ϊ��---
		#define COLOUR_MLIST_DEFAULT_0  0xfd80 //����ɫ0 000  //����ɫĬ��Ϊ0
		#define COLOUR_MLIST_DEFAULT_1  0x4a48 //����ɫ1 001
		#define COLOUR_MLIST_DEFAULT_2  0xaa05 //����ɫ2 010
		#define COLOUR_MLIST_DEFAULT_3  0x3186 //����ɫ3 011
		#define COLOUR_MLIST_DEFAULT_4  0xef77 //����ɫ0 100
		#define COLOUR_MLIST_DEFAULT_5  0x0000 //����ɫ1 101
		#define COLOUR_MLIST_DEFAULT_6  0xffdb //����ɫ2 110
		#define COLOUR_MLIST_DEFAULT_7  0x3c57 //����ɫ3 111
		#define COLOUR_MLIST_NORMAL_TEXT      (1)//�˵��б�����ɫ(���ɫȡ��,��6)
		#define COLOUR_MLIST_WSLIDER_NUM      (1)//�˵��б�wSlider������ɫ
		#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//�˵��б�wMessage�ַ���ɫ
		#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//�˵��б�wCheckbox�����ɫ
		#define COLOUR_MLIST_WCHECKBOX_INTER  (1)//�˵��б�wCheckbox�������ɫ
		#define COLOUR_MLIST_SCROOL_BAR       (1)//�˵��б��������ɫ
		#define COLOUR_MESS_TIP_BOX_BORDER    (3)//wMessage��ʾ������Ե��ɫ
		#define COLOUR_MESS_TIP_TEXT          (3)//wMessage��ʾ����������ɫ
		#define COLOUR_SLIDER_TIP_BOX_BORDER  (3)//wSlider����������Ե��ɫ
		#define COLOUR_SLIDER_TIP_TEXT        (3)//wSlider��������������ɫ
		#define COLOUR_SLIDER_TIP_NUM         (4)//wSlider��������������ɫ
		#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider���������Ļ����߿���ɫ
		#define COLOUR_SLIDER_TIP_BAR_PROG    (2)//wSlider���������Ľ�����ɫ
		#define COLOUR_DEBUG_BAR_BG           (0)//���Դ��ĵ�ɫ
		#define COLOUR_DEBUG_BAR_BORDER       (1)//���Դ��ı߿���ɫ
		#define COLOUR_DEBUG_BAR_TEXT         (1)//���Դ���������ɫ
	#else
		#error("not support LCD_COLOUR_BIT!")
	#endif
#endif




















//------------����-----------

//�ǲ���,Ĭ��ʹ��1λɫ
#ifndef LCD_USE_RGB565
	#ifdef LCD_COLOUR_BIT
		#undef LCD_COLOUR_BIT
	#endif
	#define LCD_COLOUR_BIT (1)
	#define COLOUR_MLIST_NORMAL_TEXT      (1)//�˵��б�����ɫ
	#define COLOUR_MLIST_WSLIDER_NUM      (1)//�˵��б�wSlider������ɫ
	#define COLOUR_MLIST_WMESSAGE_TEXT    (1)//�˵��б�wMessage�ַ���ɫ
	#define COLOUR_MLIST_WCHECKBOX_BORDER (1)//�˵��б�wCheckbox�����ɫ
	#define COLOUR_MLIST_WCHECKBOX_INTER  (1)//�˵��б�wCheckbox�������ɫ
	#define COLOUR_MLIST_SCROOL_BAR       (1)//�˵��б��������ɫ
	#define COLOUR_MESS_TIP_BOX_BORDER    (1)//wMessage��ʾ������Ե��ɫ
	#define COLOUR_MESS_TIP_TEXT          (1)//wMessage��ʾ����������ɫ
	#define COLOUR_SLIDER_TIP_BOX_BORDER  (1)//wSlider����������Ե��ɫ
	#define COLOUR_SLIDER_TIP_TEXT        (1)//wSlider��������������ɫ
	#define COLOUR_SLIDER_TIP_NUM         (1)//wSlider��������������ɫ
	#define COLOUR_SLIDER_TIP_BAR_BORDER  (1)//wSlider���������Ļ����߿���ɫ
	#define COLOUR_SLIDER_TIP_BAR_PROG    (1)//wSlider���������Ľ�����ɫ
	#define COLOUR_DEBUG_BAR_BG           (0)//���Դ��ĵ�ɫ
	#define COLOUR_DEBUG_BAR_BORDER       (1)//���Դ��ı߿���ɫ
	#define COLOUR_DEBUG_BAR_TEXT         (1)//���Դ���������ɫ
#endif







#if (WEGUI_INTERFACE_PORT ==_DEMO_4KEY_PORT)    //4������ģʽ
	#define Wegui_Interface_port_Init() do{}while(0)
	#define Wegui_Interface_stick(x) do{pc_4key_interface();}while(0)
#else
	#warning("no interface.c")
	#define Wegui_Interface_port_Init() do{}while(0)
	#define Wegui_Interface_stick(x) do{pc_none_interface();}while(0)
#endif



#define Wegui_Uart_Port_Init() do{}while(0) //��ֹ�������
#define Wegui_uart_rx_stick(stick) do{}while(0) //��ֹ�������


#include "lcd_wegui_menu_app.h"


#endif
