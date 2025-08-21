


#include "SDL.h"

#include "lcd_driver_config.h"
#include "lcd_wegui_config.h"
#include "pc_interface.h"
#include "pc_lcd_port.h"
#include "user_wegui_menu.h"
#include "lcd_wegui_menu_mlist.h"
#include "lcd_wegui_tip.h"




void pc_4key_interface()//按键鼠标
{
    //按键鼠标
    SDL_Event i;
    char key=0;
    while (SDL_PollEvent(&i) != 0)
    {
        switch (i.type)
        {
            case SDL_KEYDOWN:
            {
                printf("\nk=%s",  SDL_GetKeyName(i.key.keysym.sym));
                const Uint8 *state = SDL_GetKeyboardState(NULL);
                //-----------------------------------------上----------------------------------------
                if ((state[SDL_SCANCODE_UP])||(state[SDL_SCANCODE_W]))
                {
                    switch (Wegui.tip.state)
                    {
                        case FREE:break;//弹窗已退出
                        case EXITING:break;//弹窗已退出
                        case ENTERING:
                        case DISPLAYING:
                        {
                            switch (Wegui.tip.type)
                            {
                                case message:Wegui_Tip_Quit();break;
                                case slider:Wegui_Tip_Save_andQuit();break;
                                default:break;
                            }
                            return;//优先处理弹窗,不处理菜单
                        }
                    }
                    switch(Wegui.menu->menuType)
                    {
                        case mList:Wegui_mlist_cursor_Prev();break;
                        case mPorgram:  //自定义功能APP
                            //if((Wegui.menu == &m_App_UartScreen) || (Wegui.menu == &m_App_ThemeClock))
                            {
                                Wegui_mlist_Back_menu();;
                            }break;
                        default:break;
                    }
                }
                //-----------------------------------------下----------------------------------------
                if ((state[SDL_SCANCODE_DOWN])||(state[SDL_SCANCODE_S]))
                {
                    switch (Wegui.tip.state)
                    {
                        case FREE:break;//弹窗已退出
                        case EXITING:break;//弹窗已退出
                        case ENTERING:
                        case DISPLAYING:
                        {
                            switch (Wegui.tip.type)
                            {
                                case message:Wegui_Tip_Quit();break;
                                default:break;
                            }
                            return;//优先处理弹窗,不处理菜单
                        }
                    }
                    switch(Wegui.menu->menuType)
                    {
                        case mList:Wegui_mlist_cursor_Next();break;
                        case mPorgram:  //自定义功能APP
                            //if((Wegui.menu == &m_App_UartScreen) || (Wegui.menu == &m_App_ThemeClock))
                            {
                                Wegui_mlist_Back_menu();;
                            }break;
                        default:break;
                    }
                }
                //-----------------------------------------左----------------------------------------
                if ((state[SDL_SCANCODE_LEFT])||(state[SDL_SCANCODE_A]))
                {
                    switch (Wegui.tip.state)
                    {
                        case FREE:break;//弹窗已退出
                        case EXITING:break;//弹窗已退出
                        case ENTERING:
                        case DISPLAYING:
                        {
                            switch (Wegui.tip.type)
                            {
                                case message:Wegui_Tip_Quit();break;
                                case slider:Wegui_Tip_Value_Dec();break;
                                default:break;
                            }
                            return;//优先处理弹窗,不处理菜单
                        }
                    }
                    switch(Wegui.menu->menuType)
                    {
                        case mList:Wegui_mlist_Back_menu();break;
                        case mPorgram:  //自定义功能APP

                            {
                                Wegui_mlist_Back_menu();
                            }break;
                        default:break;
                    }
                }
                //-----------------------------------------右----------------------------------------
                if ((state[SDL_SCANCODE_RIGHT])||(state[SDL_SCANCODE_D]))
                {
                    switch (Wegui.tip.state)
                    {
                        case FREE:break;//弹窗已退出
                        case EXITING:break;//弹窗已退出
                        case ENTERING:
                        case DISPLAYING:
                        {
                            switch (Wegui.tip.type)
                            {
                                case message:Wegui_Tip_Quit();break;
                                case slider:Wegui_Tip_Value_Add();break;
                                default:break;
                            }
                            return;//优先处理弹窗,不处理菜单
                        }
                    }
                    switch(Wegui.menu->menuType)
                    {
                        case mList:Wegui_mlist_Enter_cursor();break;
                        case mPorgram:  //自定义功能APP

                            {
                                Wegui_mlist_Back_menu();
                            }break;
                        default:break;
                    }
                }
            }break;
            case SDL_KEYUP:
                break;
            case SDL_TEXTINPUT:
                break;
            case SDL_QUIT:
                sdl_quit = 1;
                break;
            default:
                break;
        }
    }
}



void pc_none_interface()//按键鼠标
{
    //按键鼠标
    SDL_Event i;
    while (SDL_PollEvent(&i) != 0)
    {

    }
}
