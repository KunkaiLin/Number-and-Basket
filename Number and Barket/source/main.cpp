/*******************************************************************
此文件是主程序。
时间：2018/11/8
********************************************************************/

#include "game.h"

int main( int argc, char **args )									// 主函数 
{
	srand( unsigned(time(0)));										// 置随机数种子 
	
	init_SDL();														// 初始化 SDL 的各个部件 
	
	bool quit = false;												// 是否退出游戏 
	SDL_Event event;												// 事件 
	Game game;
	
	while( !quit )													// 如果没退出 
	{
		game.reset_data();											// 重置游戏数据 
		if( !game.show_begin() )									// 若在游戏选择时退出游戏 
		{
			break;													// 则退出 
		}
		
		while( !game.is_end() )										// 如果游戏没结束 
		{
			if( SDL_PollEvent( &event ) )							// 查看是否有事件未处理 
			{
				if( event.type == SDL_QUIT )						// 叉掉窗口 
				{
					quit = true;
					break;
				}
				game.handle_input( event );							// 处理输入 
			}
			game.game_run();										// 游戏运行 
		}
		SDL_Delay(1000);
		if( !quit && !game.show_end() )								// 
		{
			break;
		}
	}
	
	quit_SDL();														// 退出 SDL 
	
	return 0;
}
