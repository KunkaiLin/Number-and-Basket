/*******************************************************************
此文件包含：
	1. 需要用到的所有头文件;
	2. 需要用到的常量;
	3. 全局函数的声明; 
	4. 所用的类的定义。
时间：2018/11/8 
********************************************************************/
#ifndef _GAME_H_ 
#define _GAME_H_

/***********************头文件**************************************/ 
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

/***********************常量定义************************************/ 
const int SCREEN_WIDTH = 1200;										// 屏幕宽度 
const int SCREEN_HEIGHT = 650;										// 屏幕高度 
const int SCREEN_BPP = 32;											// 颜色深度 

const int FPS = 30;													// 帧率 

const int BASKET_HEIGHT = 74;										// 篮子高度 

const int NUMBER_NUM = 10;											// 数字个数 
const int NUMBER_WIDTH = 25;										// 数字宽度 
const int NUMBER_HEIGHT = 33;										// 数字高度 
const int EXP_BEGIN = SCREEN_WIDTH / NUMBER_NUM / 2;				// 式子开始位置 
const int EXP_MARGIN = SCREEN_WIDTH / NUMBER_NUM;					// 式子之间的间隔 
const int EXP_TIME_INTERVAL = 5000;									// 两个式子掉落的间隔 
																	// 式子最大能到达的地方 
const int EXP_MAX_HEIGHT = SCREEN_HEIGHT - NUMBER_HEIGHT - BASKET_HEIGHT + 10;

const int GAME_TIME = 60 * 1000;									// 游戏时间  

const int START_X = 484;											// 开始按钮的 x 坐标 
const int START_Y = 330;											// 开始按钮的 y 坐标

const int RETURN_X = 532;											// 返回按钮的 x 坐标 
const int RETURN_Y = 380;											// 返回按钮的 y 坐标

const SDL_Color TIME_COLOR = {0xff, 0x33, 0x33};					// 时间的颜色 
const SDL_Color SCORE_COLOR = {0x00, 0x66, 0xff};					// 得分的颜色 

const string font_path = "..\\font\\";								// 字体文件路径 
const string pic_path = "..\\pic\\";								// 图片文件路径 
const string sound_path = "..\\sound\\";							// 声音文件路径 

extern SDL_Surface* screen;											// 全局变量，screen 

/***********************全局函数声明********************************/ 

SDL_Surface* load_image( string filename );							// 加载一张图片 
																	//  
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination , SDL_Rect *clip = NULL );

bool init_SDL();

bool quit_SDL();

int random( int min, int max );

/***********************Timer类定义*********************************/
class Timer{
	private:
		int startTicks;												// 计时开始时的时间 
		int pausedTicks;											// 暂停时的时间 
		bool paused;												// 是否暂停 
		bool started;												// 是否开始计时 
	public:
		Timer();													// 构造函数 
		void start();												// 开始计时 
		void stop();												// 停止计时 
		void pause();												// 暂停计时 
		void unpause();												// 恢复计时 
		int get_ticks();											// 获得计时开始到现在过了多少时间 
		bool is_started();											// 是否开始 
		bool is_paused();											// 是否暂停 
};

/***********************Game类定义**********************************/
class Game
{
	private:
		bool end;													// 游戏是否结束	
		int answer;													// 生成的式子的答案 
		int reply;													// 玩家的回答 
		int score;													// 分数 
		int exp_x;													// 式子显示位置的 x 坐标 
		int exp_y;													// 式子显示位置的 y 坐标
		double adjust;												// 调整式子下落速度 
		string expression;											// 生成的式子 
		SDL_Surface *num[NUMBER_NUM];								// 存储式子中的数字 
		SDL_Surface *time_num[NUMBER_NUM];							// 开头倒计时的 3 2 1 
		SDL_Surface *basket[NUMBER_NUM];							// 存储篮子 
		SDL_Surface *background;									// 背景图片 
		SDL_Surface *exp_bg;										// 式子的背景图片，一张全透明的图像 
		SDL_Surface *play[2];										// 存储"开始游戏"按钮 
		SDL_Surface *plus;											// 加号 
		SDL_Surface *minus;											// 减号 
		SDL_Surface *end_tip;										// 游戏结束的提示语
		SDL_Surface *author;										// 作者
		SDL_Surface *ret_btn[2];									// 返回按钮 
		Mix_Chunk *correct;											// 答对的音效 
		Mix_Chunk *error;											// 答错的音效
		Mix_Chunk *start_time;										// 倒计时音效
		Mix_Chunk *start;											// 倒计时音效				
		Mix_Music *game_music;										// 游戏的背景音乐 
		TTF_Font *font;												// 使用的字体 
		Timer fps;													// 用来控制帧率 
		Timer game_timer;											// 游戏的倒计时 
	public:
		Game();														// 构造函数 
		~Game();													// 析构函数 
		void reset_data(); 											// 重置一些数据 
		bool show_begin();											// 显示开始游戏的画面 
		void show_game();											// 显示游戏过程的画面 
		bool show_end(); 											// 显示结束画面 
		bool is_end();												// 游戏是否结束 
		void create_exp();											// 生成一个式子			
		void setup_exp();											// 将生成的式子转换成图片 
		void move_exp(int key);										// 式子移动 
		void handle_input( const SDL_Event &event );				// 处理输入
		bool wait_click(SDL_Surface* s1, SDL_Surface* s2, int a, int b); 
		void game_run();											// 游戏主要运行程序 
		void control_fps();											// 控制游戏帧率 
};

#endif
