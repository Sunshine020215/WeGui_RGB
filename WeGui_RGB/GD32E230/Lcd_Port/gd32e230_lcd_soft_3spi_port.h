/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#ifndef STM32F103_LCD_SOFT_3SPI_PORT_H
#define STM32F103_LCD_SOFT_3SPI_PORT_H

#include "gd32e23x.h"


/*------------------------------------------------------
** 但在屏幕通讯速率耐受较低时,建议使用"库函数"操作IO通讯
** 但在屏幕通讯速率耐受较高时,建议使用"寄存器"操作IO通讯
------------------------------------------------------*/



//-----------------IO接口定义---------------- 

//-----SCL-----
//#define LCD_SCL_Clr() do{gpio_bit_reset(GPIOA,GPIO_PIN_5);}while(0)//库函数操作IO
//#define LCD_SCL_Set() do{gpio_bit_set(GPIOA,GPIO_PIN_5);}while(0)//库函数操作IO
#define LCD_SCL_Clr() do{GPIO_BC(GPIOA) = (uint32_t)GPIO_PIN_5;;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SCL_Set() do{GPIO_BOP(GPIOA) = (uint32_t)GPIO_PIN_5;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SCL_IO_Init() \
	do{\
		rcu_periph_clock_enable(RCU_GPIOA);\
		gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_5);\
		gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);\
	}while(0)

//-----SDA-----
//#define LCD_SDA_Clr() do{gpio_bit_reset(GPIOA,GPIO_PIN_7);}while(0)//库函数操作IO
//#define LCD_SDA_Set() do{gpio_bit_set(GPIOA,GPIO_PIN_7);}while(0)//库函数操作IO
#define LCD_SDA_Clr() do{GPIO_BC(GPIOA) = (uint32_t)GPIO_PIN_7;;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SDA_Set() do{GPIO_BOP(GPIOA) = (uint32_t)GPIO_PIN_7;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SDA_IO_Init() \
	do{\
		rcu_periph_clock_enable(RCU_GPIOA);\
		gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7);\
		gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);\
		LCD_SDA_Set();\
	}while(0)

//-----RES-----
//#define LCD_RES_Clr() do{gpio_bit_reset(GPIOB,GPIO_PIN_10);}while(0)//库函数操作IO
//#define LCD_RES_Set() do{gpio_bit_set(GPIOB,GPIO_PIN_10);}while(0)//库函数操作IO
#define LCD_RES_Clr() do{GPIO_BC(GPIOB) = (uint32_t)GPIO_PIN_10;;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_RES_Set() do{GPIO_BOP(GPIOB) = (uint32_t)GPIO_PIN_10;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_RES_IO_Init() \
	do{\
		rcu_periph_clock_enable(RCU_GPIOB);\
		gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_10);\
		gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);\
		LCD_RES_Set();\
	}while(0)



//-----CS-----(可选)
//#define LCD_CS_Clr()  do{gpio_bit_reset(GPIOA,GPIO_PIN_6);}while(0)//库函数操作IO
//#define LCD_CS_Set()  do{gpio_bit_set(GPIOA,GPIO_PIN_6);}while(0)//库函数操作IO
#define LCD_CS_Clr() do{GPIO_BC(GPIOA) = (uint32_t)GPIO_PIN_6;;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_CS_Set() do{GPIO_BOP(GPIOA) = (uint32_t)GPIO_PIN_6;}while(0)//直接寄存器操作,节省函数调用时间

#define LCD_CS_IO_Init() \
	do{\
		rcu_periph_clock_enable(RCU_GPIOA);\
		gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6);\
		gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);\
		LCD_CS_Set();\
	}while(0)
	
//-----BL-----(可选/BL背光一般RGB TFT屏幕专有)
//#define LCD_BL_Clr()  do{gpio_bit_reset(GPIOB,GPIO_PIN_0);}while(0)//库函数操作IO
//#define LCD_BL_Set()  do{gpio_bit_set(GPIOB,GPIO_PIN_0);}while(0)//库函数操作IO
#define LCD_BL_Clr() do{GPIO_BC(GPIOB) = (uint32_t)GPIO_PIN_0;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_BL_Set() do{GPIO_BOP(GPIOB) = (uint32_t)GPIO_PIN_0;}while(0)//直接寄存器操作,节省函数调用时间
//#define LCD_BL_On() do{LCD_BL_Clr();}while(0)
//#define LCD_BL_Off() do{LCD_BL_Set();}while(0)
#define LCD_BL_On() do{LCD_BL_Set();}while(0)
#define LCD_BL_Off() do{LCD_BL_Clr();}while(0)
#define LCD_BL_IO_Init() \
	do{\
		rcu_periph_clock_enable(RCU_GPIOB);\
		gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0);\
		gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);\
		LCD_BL_On();\
	}while(0)

//-----DC-----
//----3SPI无需DC接口-------
//#define LCD_DC_Clr()  do{gpio_bit_reset(GPIOB,GPIO_PIN_11);}while(0)//库函数操作IO
//#define LCD_DC_Set()  do{gpio_bit_set(GPIOB,GPIO_PIN_11);}while(0)//库函数操作IO
//#define LCD_DC_Clr() do{GPIO_BC(GPIOB) = (uint32_t)GPIO_PIN_11;;}while(0)//直接寄存器操作,节省函数调用时间
//#define LCD_DC_Set() do{GPIO_BOP(GPIOB) = (uint32_t)GPIO_PIN_11;}while(0)//直接寄存器操作,节省函数调用时间
//#define LCD_DC_IO_Init() \
//	do{\
//		rcu_periph_clock_enable(RCU_GPIOB);\
//		gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_11);\
//		gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);\
//		LCD_DC_Set();\
//	}while(0)
	
	
	
#define LCD_is_Busy() (0)
void LCD_delay_ms(volatile uint32_t ms);
void LCD_Port_Init(void);//接口初始化	
void LCD_Send_1Cmd(uint8_t dat);//向屏幕发送1个命令
void LCD_Send_1Dat(uint8_t dat);//向屏幕发送1个数据
void LCD_Send_nDat(uint8_t *p,uint16_t num);//向屏幕发送num个数据
void LCD_Send_nCmd(uint8_t *p,uint16_t num);//向屏幕发送num个命令
uint8_t LCD_Refresh(void);
	
	
#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
	void LCD_Reset_crc(void);//刷新一次crc值, 用于强制刷新屏幕,防止动态刷新出现区域不刷新
#endif
		
#endif

