/*
	Copyright 2025 Lu Zhihao
	本程序仅供学习用途, 暂不公开对其他用途的授权
*/

#include "lcd_driver_config.h"
#include "lcd_wegui_config.h"

#if (LCD_PORT == _PC)

#include "stdint.h"
#include "wchar.h"
#include "stdio.h"
#include "pc_lcd_port.h"
#include "lcd_driver.h"
#include "SDL.h"


SDL_Window* window;
SDL_Renderer* renderer;
uint8_t sdl_quit=0;

void PC_SDL2_Init()
{
    sdl_quit = 0;

    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
    }
    // 创建窗口
    window = SDL_CreateWindow("WeGui simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*WIN_ZOOM, SCREEN_HIGH*WIN_ZOOM, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
    }
    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    //1.清空渲染器
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);//笔刷着色
	SDL_RenderClear(renderer);
	//2.渲染器发送到屏幕
    SDL_RenderPresent(renderer);

}


uint8_t LCD_Refresh(void)
{
	uint16_t x,y;
	//1.清空渲染器
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);//笔刷着色
    SDL_RenderClear(renderer);


	for(y=0;y<SCREEN_HIGH;y++)
	{
		uint8_t ypage=y/8;
		uint8_t mask=0x01<<(y%8);
		uint8_t *gram=&lcd_driver.LCD_GRAM[ypage][0][0];
		for(x=0;x<SCREEN_WIDTH;x++)
		{
			uint8_t c=0;
			#if (LCD_COLOUR_BIT>=1)//---1位色---
				if(*gram++&mask){c+=1;}
			#endif
			#if (LCD_COLOUR_BIT>=2)//---2位色---
				if(*gram++&mask){c+=2;}
			#endif
			#if (LCD_COLOUR_BIT>=3)//---3位色---
				if(*gram++&mask){c+=4;}
			#endif

			//2.渲染器画点
			SDL_SetRenderDrawColor(renderer,
                                    (lcd_driver.colour[c] & 0xf800)    >> 8,
                                    (lcd_driver.colour[c] & 0x07e0)  >> 3,
                                    (lcd_driver.colour[c] & 0x001f)   << 3,
                                    0xff);
            for(uint8_t zy=0;zy<WIN_ZOOM;zy++)//像素放大
            {
                for(uint8_t zx=0;zx<WIN_ZOOM;zx++)
                {
                    SDL_RenderDrawPoint(renderer, x*WIN_ZOOM+zx,y*WIN_ZOOM+zy);
                }
            }
		}
	}
	//3.渲染器发送到屏幕
    SDL_RenderPresent(renderer);
	return 0;
}


#endif
