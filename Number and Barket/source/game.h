/*******************************************************************
���ļ�������
	1. ��Ҫ�õ�������ͷ�ļ�;
	2. ��Ҫ�õ��ĳ���;
	3. ȫ�ֺ���������; 
	4. ���õ���Ķ��塣
ʱ�䣺2018/11/8 
********************************************************************/
#ifndef _GAME_H_ 
#define _GAME_H_

/***********************ͷ�ļ�**************************************/ 
#include "SDL/SDL.h"											
#include "SDL/SDL_image.h"										
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

/***********************��������************************************/ 
const int SCREEN_WIDTH = 1200;										// ��Ļ��� 
const int SCREEN_HEIGHT = 650;										// ��Ļ�߶� 
const int SCREEN_BPP = 32;											// ��ɫ��� 

const int FPS = 30;													// ֡�� 

const int BASKET_HEIGHT = 74;										// ���Ӹ߶� 

const int NUMBER_NUM = 10;											// ���ָ��� 
const int NUMBER_WIDTH = 25;										// ���ֿ�� 
const int NUMBER_HEIGHT = 33;										// ���ָ߶� 
const int EXP_BEGIN = SCREEN_WIDTH / NUMBER_NUM / 2;				// ʽ�ӿ�ʼλ�� 
const int EXP_MARGIN = SCREEN_WIDTH / NUMBER_NUM;					// ʽ��֮��ļ�� 
const int EXP_TIME_INTERVAL = 5000;									// ����ʽ�ӵ���ļ�� 
																	// ʽ������ܵ���ĵط� 
const int EXP_MAX_HEIGHT = SCREEN_HEIGHT - NUMBER_HEIGHT - BASKET_HEIGHT + 10;

const int GAME_TIME = 60 * 1000;									// ��Ϸʱ��  

const int START_X = 484;											// ��ʼ��ť�� x ���� 
const int START_Y = 330;											// ��ʼ��ť�� y ����

const int RETURN_X = 532;											// ���ذ�ť�� x ���� 
const int RETURN_Y = 380;											// ���ذ�ť�� y ����

const SDL_Color TIME_COLOR = {0xff, 0x33, 0x33};					// ʱ�����ɫ 
const SDL_Color SCORE_COLOR = {0x00, 0x66, 0xff};					// �÷ֵ���ɫ 

const string font_path = "..\\font\\";								// �����ļ�·�� 
const string pic_path = "..\\pic\\";								// ͼƬ�ļ�·�� 
const string sound_path = "..\\sound\\";							// �����ļ�·�� 

extern SDL_Surface* screen;											// ȫ�ֱ�����screen 

/***********************ȫ�ֺ�������********************************/ 

SDL_Surface* load_image( string filename );							// ����һ��ͼƬ 
																	//  
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination , SDL_Rect *clip = NULL );

bool init_SDL();

bool quit_SDL();

int random( int min, int max );

/***********************Timer�ඨ��*********************************/
class Timer{
	private:
		int startTicks;												// ��ʱ��ʼʱ��ʱ�� 
		int pausedTicks;											// ��ͣʱ��ʱ�� 
		bool paused;												// �Ƿ���ͣ 
		bool started;												// �Ƿ�ʼ��ʱ 
	public:
		Timer();													// ���캯�� 
		void start();												// ��ʼ��ʱ 
		void stop();												// ֹͣ��ʱ 
		void pause();												// ��ͣ��ʱ 
		void unpause();												// �ָ���ʱ 
		int get_ticks();											// ��ü�ʱ��ʼ�����ڹ��˶���ʱ�� 
		bool is_started();											// �Ƿ�ʼ 
		bool is_paused();											// �Ƿ���ͣ 
};

/***********************Game�ඨ��**********************************/
class Game
{
	private:
		bool end;													// ��Ϸ�Ƿ����	
		int answer;													// ���ɵ�ʽ�ӵĴ� 
		int reply;													// ��ҵĻش� 
		int score;													// ���� 
		int exp_x;													// ʽ����ʾλ�õ� x ���� 
		int exp_y;													// ʽ����ʾλ�õ� y ����
		double adjust;												// ����ʽ�������ٶ� 
		string expression;											// ���ɵ�ʽ�� 
		SDL_Surface *num[NUMBER_NUM];								// �洢ʽ���е����� 
		SDL_Surface *time_num[NUMBER_NUM];							// ��ͷ����ʱ�� 3 2 1 
		SDL_Surface *basket[NUMBER_NUM];							// �洢���� 
		SDL_Surface *background;									// ����ͼƬ 
		SDL_Surface *exp_bg;										// ʽ�ӵı���ͼƬ��һ��ȫ͸����ͼ�� 
		SDL_Surface *play[2];										// �洢"��ʼ��Ϸ"��ť 
		SDL_Surface *plus;											// �Ӻ� 
		SDL_Surface *minus;											// ���� 
		SDL_Surface *end_tip;										// ��Ϸ��������ʾ��
		SDL_Surface *author;										// ����
		SDL_Surface *ret_btn[2];									// ���ذ�ť 
		Mix_Chunk *correct;											// ��Ե���Ч 
		Mix_Chunk *error;											// ������Ч
		Mix_Chunk *start_time;										// ����ʱ��Ч
		Mix_Chunk *start;											// ����ʱ��Ч				
		Mix_Music *game_music;										// ��Ϸ�ı������� 
		TTF_Font *font;												// ʹ�õ����� 
		Timer fps;													// ��������֡�� 
		Timer game_timer;											// ��Ϸ�ĵ���ʱ 
	public:
		Game();														// ���캯�� 
		~Game();													// �������� 
		void reset_data(); 											// ����һЩ���� 
		bool show_begin();											// ��ʾ��ʼ��Ϸ�Ļ��� 
		void show_game();											// ��ʾ��Ϸ���̵Ļ��� 
		bool show_end(); 											// ��ʾ�������� 
		bool is_end();												// ��Ϸ�Ƿ���� 
		void create_exp();											// ����һ��ʽ��			
		void setup_exp();											// �����ɵ�ʽ��ת����ͼƬ 
		void move_exp(int key);										// ʽ���ƶ� 
		void handle_input( const SDL_Event &event );				// ��������
		bool wait_click(SDL_Surface* s1, SDL_Surface* s2, int a, int b); 
		void game_run();											// ��Ϸ��Ҫ���г��� 
		void control_fps();											// ������Ϸ֡�� 
};

#endif
