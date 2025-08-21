#ifndef GD32E230_LCD_SOFT_I2C_PORT_H
#define GD32E230_LCD_SOFT_I2C_PORT_H

#include "gd32e23x.h"



/*------------------------------------------------------
** 标准情况下,I2C最大通讯速率控制在400k
** 在屏幕通讯速率耐受较低时,建议使用"库函数"操作IO通讯,亦可减小上拉电阻
** 在屏幕通讯速率耐受较高时,建议使用"寄存器"操作IO通讯,亦可增加I2C驱动延时

------------------------------------------------------*/

/*------------------------------------------------------
** 建议在主频>24MHz时,建议使用"库函数"操作IO通讯
** 建议在主频=24MHz时,建议使用"寄存器"操作IO通讯
------------------------------------------------------*/



//-----------------IO接口定义---------------- 

//-----SCL-----
//#define LCD_SCL_Clr() do{gpio_bit_reset(GPIOB,GPIO_PIN_10);}while(0)//库函数操作IO
//#define LCD_SCL_Set() do{gpio_bit_set(GPIOB,GPIO_PIN_10);}while(0)//库函数操作IO
#define LCD_SCL_Clr() do{GPIO_BC(GPIOB) = (uint32_t)GPIO_PIN_10;;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SCL_Set() do{GPIO_BOP(GPIOB) = (uint32_t)GPIO_PIN_10;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SCL_IO_Init() \
	do{\
		rcu_periph_clock_enable(RCU_GPIOB);\
		gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_10);\
		gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);\
	}while(0)

//-----SDA-----
//#define LCD_SDA_Clr() do{gpio_bit_reset(GPIOB,GPIO_PIN_11);}while(0)//库函数操作IO
//#define LCD_SDA_Set() do{gpio_bit_set(GPIOB,GPIO_PIN_11);}while(0)//库函数操作IO
#define LCD_SDA_Clr() do{GPIO_BC(GPIOB) = (uint32_t)GPIO_PIN_11;;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SDA_Set() do{GPIO_BOP(GPIOB) = (uint32_t)GPIO_PIN_11;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_SDA_IO_Init() \
	do{\
		rcu_periph_clock_enable(RCU_GPIOB);\
		gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_11);\
		gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);\
		LCD_SDA_Set();\
	}while(0)
	
//-----RES-----(部分屏幕可选)
//#define LCD_RES_Clr() do{gpio_bit_reset(GPIOA,GPIO_PIN_6);}while(0)//库函数操作IO
//#define LCD_RES_Set() do{gpio_bit_set(GPIOA,GPIO_PIN_6);}while(0)//库函数操作IO
#define LCD_RES_Clr() do{GPIO_BC(GPIOA) = (uint32_t)GPIO_PIN_6;;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_RES_Set() do{GPIO_BOP(GPIOA) = (uint32_t)GPIO_PIN_6;}while(0)//直接寄存器操作,节省函数调用时间
#define LCD_RES_IO_Init() \
	do{\
		rcu_periph_clock_enable(RCU_GPIOA);\
		gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6);\
		gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);\
		LCD_RES_Set();\
	}while(0)

	
	
#define LCD_is_Busy() (0)
void LCD_delay_ms(volatile uint32_t ms);
void LCD_Send_1Cmd(uint8_t dat);//向屏幕发送1个命令
void LCD_Send_1Dat(uint8_t dat);//向屏幕发送1个数据
void LCD_Send_nDat(uint8_t *p,uint16_t num);//向屏幕发送num个数据
void LCD_Send_nCmd(uint8_t *p,uint16_t num);//向屏幕发送num个命令
void LCD_Port_Init(void);//接口初始化	
uint8_t LCD_Refresh(void);

#if ((LCD_MODE == _FULL_BUFF_DYNA_UPDATE) || (LCD_MODE == _PAGE_BUFF_DYNA_UPDATE))
	void LCD_Reset_crc(void);//刷新一次crc值, 用于强制刷新屏幕,防止动态刷新出现区域不刷新
#endif

#endif
	
	
	
