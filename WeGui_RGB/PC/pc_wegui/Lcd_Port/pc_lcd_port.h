/*
	Copyright 2025 Lu Zhihao
*/

#ifndef PC_LCD_PORT_H
#define PC_LCD_PORT_H

#include "stdint.h"



#ifdef __cplusplus
extern "C"{
#endif




#define LCD_is_Busy() (0)
extern uint8_t sdl_quit;
void PC_SDL2_Init();
void LCD_delay_ms(volatile uint32_t ms);
uint8_t LCD_Refresh(void);


#ifdef __cplusplus
}
#endif




#endif


