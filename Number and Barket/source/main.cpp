/*******************************************************************
���ļ���������
ʱ�䣺2018/11/8
********************************************************************/

#include "game.h"

int main( int argc, char **args )									// ������ 
{
	srand( unsigned(time(0)));										// ����������� 
	
	init_SDL();														// ��ʼ�� SDL �ĸ������� 
	
	bool quit = false;												// �Ƿ��˳���Ϸ 
	SDL_Event event;												// �¼� 
	Game game;
	
	while( !quit )													// ���û�˳� 
	{
		game.reset_data();											// ������Ϸ���� 
		if( !game.show_begin() )									// ������Ϸѡ��ʱ�˳���Ϸ 
		{
			break;													// ���˳� 
		}
		
		while( !game.is_end() )										// �����Ϸû���� 
		{
			if( SDL_PollEvent( &event ) )							// �鿴�Ƿ����¼�δ���� 
			{
				if( event.type == SDL_QUIT )						// ������� 
				{
					quit = true;
					break;
				}
				game.handle_input( event );							// �������� 
			}
			game.game_run();										// ��Ϸ���� 
		}
		SDL_Delay(1000);
		if( !quit && !game.show_end() )								// 
		{
			break;
		}
	}
	
	quit_SDL();														// �˳� SDL 
	
	return 0;
}
