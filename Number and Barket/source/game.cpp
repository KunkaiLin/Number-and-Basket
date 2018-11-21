/*******************************************************************
此文件包含：
	1. 全局函数的实现； 
	2. 所用的类的具体实现. 
时间：2018/11/8
********************************************************************/

#include "game.h"

SDL_Surface* screen;

/***********************全局函数实现开始****************************/

SDL_Surface* load_image( string filename )							// 加载一张图片 
{
    SDL_Surface* loadedImage = NULL;

    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );

        SDL_FreeSurface( loadedImage );

        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0, 0 ) );
        }
    }

    return optimizedImage;
}
																	// 将 source 贴到 destination 
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination , SDL_Rect *clip )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init_SDL()														// 初始化 SDL 及其部件 
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    if( screen == NULL )
    {
        return false;
    }
    
	if( TTF_Init() == -1 )
	{
		return false;
	}
	
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }
    
    SDL_WM_SetCaption( "Number and Basket", NULL );					// 设置标题 
	SDL_WM_SetIcon(SDL_LoadBMP(string(pic_path + "icon.bmp").c_str()), NULL);				// 设置图标 
	
    return true;
}

bool quit_SDL()														// 退出 SDL 
{
	Mix_CloseAudio();
	
	SDL_Quit();
}

int random( int min, int max )										// 返回 min~max 之间的一个随机数 
{
	if( min > max )
		return 0;
	return rand() % ( max - min + 1 ) + min;
}

/***********************全局函数实现结束****************************/

/***********************Timer类实现开始*****************************/ 

Timer::Timer()														// 构造
{
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}
 
void Timer::start()													// 开始计时
{
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();									// 获得当前时间 
}

void Timer::stop()													// 停止计时
{
	started = false;
	paused = false;
}

int Timer::get_ticks()												// 获得计时开始到现在过了多长时间 
{
	if( started == true )											// 若已开始计时 
	{
		if( paused == true )										// 若暂停，返回暂停时的时间 
		{
			return pausedTicks;
		}
		else
		{
			return SDL_GetTicks() - startTicks;						// 否则返回当前时间开始时时间 
		}
	}
	
	return 0;
}

void Timer::pause()													// 暂停计时 					
{
	if( ( started == true ) && ( paused == false ) )
	{
		paused = true;
		
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

void Timer::unpause()												// 恢复计时 
{
	if( paused == true )
	{
		paused = false;
		
		startTicks = SDL_GetTicks() - pausedTicks;
		
		pausedTicks = 0;
	}
}

bool Timer::is_started()											// 是否开始 
{
    return started;    
}
 
bool Timer::is_paused()												// 是否暂停 
{
    return paused;    
}
/***********************Time类实现结束******************************/

/***********************Game类实现开始******************************/

Game::Game()														// 构造函数，主要用来加载一些资源 
{
	font = TTF_OpenFont(string(font_path + "courbd.ttf").c_str(), 32 );					
	background = load_image(pic_path + "background.png");					
	plus = load_image(pic_path + "num\\plus.png");
	minus = load_image(pic_path + "num\\minus.png");
	exp_bg = load_image(pic_path + "exp_bg.png");
	play[0] = load_image(pic_path + "start_1.png");
	play[1] = load_image(pic_path + "start_2.png");
	time_num[0] = load_image(pic_path + "num\\bignum_1.png");
	time_num[1] = load_image(pic_path + "num\\bignum_2.png");
	time_num[2] = load_image(pic_path + "num\\bignum_3.png");
	end_tip = load_image(pic_path + "end_tip.png");
	author = load_image(pic_path + "author.png");
	ret_btn[0] = load_image(pic_path + "ret_btn_1.png");
	ret_btn[1] = load_image(pic_path + "ret_btn_2.png");
	correct = Mix_LoadWAV(string(sound_path + "correct.wav").c_str());
	error = Mix_LoadWAV(string(sound_path + "error.wav").c_str());
	start_time = Mix_LoadWAV(string(sound_path + "start_time.wav").c_str());
	start = Mix_LoadWAV(string(sound_path + "start.wav").c_str());
	game_music = Mix_LoadMUS(string(sound_path + "game_music.wav").c_str());
	
	for(int i = 0; i < NUMBER_NUM; i++)
	{
		ostringstream os;
		os << pic_path <<"num\\num_" << i << ".png";
		string filename = os.str();
		num[i] = load_image(filename);
		
		os.str("");
		os << pic_path <<"basket\\basket_" << i+1 << ".png";
		filename = os.str();
		basket[i] = load_image(filename);
	}
	Mix_PlayMusic( game_music, -1 );								// 播放背景音乐 
	fps.start();													// 开始计时 
}

Game::~Game()														// 析构函数，释放资源 
{
	SDL_FreeSurface( background );
	SDL_FreeSurface( plus );
	SDL_FreeSurface( minus );
	SDL_FreeSurface( exp_bg );
	SDL_FreeSurface( play[0] );
	SDL_FreeSurface( play[1] );
	SDL_FreeSurface( time_num[0] );
	SDL_FreeSurface( time_num[1] );
	SDL_FreeSurface( time_num[2] );
	SDL_FreeSurface( end_tip );
	SDL_FreeSurface( author );
	SDL_FreeSurface( ret_btn[0] );
	SDL_FreeSurface( ret_btn[1] );
	for(int i = 0; i < NUMBER_NUM; i++)
	{
		SDL_FreeSurface( num[i] );
		SDL_FreeSurface( basket[i] );
	}
	
	Mix_FreeChunk( correct );
	Mix_FreeChunk( error );
	Mix_FreeChunk( start_time );
	Mix_FreeChunk( start );
	Mix_FreeMusic( game_music );
}


void Game::reset_data()												// 重置一些数据 
{
	end = false;													 
	exp_x = 0;
	score = 0;
	adjust = 0;
	answer = 0;
	reply = 0;
	expression = "";
	SDL_FillRect(exp_bg, NULL, 0x0);
}

bool Game::show_begin()												// 显示开始画面 
{
	apply_surface(0, 0, background, screen);						// 背景 
	apply_surface((SCREEN_WIDTH-author->w) / 2, 90, author, screen);
	apply_surface(START_X, START_Y, play[0], screen);				// 开始游戏按钮 
	SDL_Flip( screen );												// 刷新屏幕 

	if( wait_click(play[0], play[1], START_X, START_Y) )
	{
		show_game();												// 显示游戏的画面 
		for(int i = 2; i >= 0; i--)									// 显示倒计时 
		{
			Mix_PlayChannel(-1, start_time, 0);
			show_game();
			apply_surface(505, 195, time_num[i], screen);
			SDL_Flip( screen );
			SDL_Delay(1000);
		}
		Mix_PlayChannel(-1, start, 0);
		SDL_Delay(500);
		create_exp();
		game_timer.start();	
		return true;
	}
		
	return false;
}

void Game::show_game()												// 显示游戏画面 
{		
	apply_surface(0, 0, background, screen);						// 背景 

	if( exp_bg )													// 显示式子 
	{
		apply_surface(exp_x, exp_y, exp_bg, screen);
		double t = (double)EXP_MAX_HEIGHT / (EXP_TIME_INTERVAL / 1000) / FPS;
		exp_y += (int)t;
		adjust += t - (int)t;
		if( adjust > 1 )
		{
			exp_y++;
			adjust -= 1;
		}
	}
	
	for(int i = 0; i < NUMBER_NUM; i++)								// 显示篮子 
	{
		apply_surface(5+i*120, SCREEN_HEIGHT-74, basket[i], screen);
	}
																	// 显示剩余时间 
	ostringstream os;
	double left_time = ((double)GAME_TIME - game_timer.get_ticks())/1000;
	if( left_time < 0 )
		left_time = 0;
	os <<"Time: " << setiosflags(ios::fixed)<<setprecision(2) << left_time << "s";
	string t = os.str();
	SDL_Surface *message = TTF_RenderText_Solid( font, t.c_str(), TIME_COLOR );
	apply_surface( 950, 30, message, screen );
																	// 显示分数 
	os.str("");
	os << "Score: " << score;
	t = os.str();
	message = TTF_RenderText_Solid( font, t.c_str(), SCORE_COLOR );
	apply_surface( 50, 30, message, screen );
	
	SDL_Flip( screen );
}

bool Game::show_end()
{
	SDL_Event event;
	ostringstream os;
	os << score;
	expression = os.str();
	setup_exp();
	bool in_button = false;
	apply_surface(0, 0, background, screen);
	apply_surface((SCREEN_WIDTH-end_tip->w)/2, (SCREEN_HEIGHT-end_tip->h)/2, end_tip, screen);
	apply_surface(620, 310, exp_bg, screen);
	apply_surface(RETURN_X, RETURN_Y, ret_btn[0], screen);
	SDL_Flip( screen );
	if( wait_click(ret_btn[0], ret_btn[1], RETURN_X, RETURN_Y) )
		return true;
	return false;
}

bool Game::wait_click(SDL_Surface* s1, SDL_Surface* s2, int a, int b)
{
	SDL_Event event; 
	bool in_button = false;											// 表示鼠标是否在按钮内 
	while( true )													// 当没有进入游戏 
	{
		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT )							// 若叉掉窗口
			{ 
				return false;
			}
			if( event.type == SDL_MOUSEBUTTONDOWN )					// 点击鼠标 
			{
				if( event.button.button == SDL_BUTTON_LEFT )		// 左键点击 
				{
					int x = event.button.x;							// 鼠标位置的 x，y 坐标 
					int y = event.button.y;							
					if( x >= a && x <= a + s1->w && y >= b && y <= b + s1->h )
					{
						return true;
					}
				}
			}
			if( event.type == SDL_MOUSEMOTION )						// 鼠标移动到开始按钮内，按钮变色 
			{
				int x = event.motion.x;
				int y = event.motion.y;
				if( x >= a && x <= a + s1->w && y >= b && y <= b + s1->h )
		        {
		        	if(!in_button)
		        	{
		        		apply_surface(a, b, s2, screen);
		        		SDL_Flip( screen ); 
		        		in_button = true;
					}
		        }
		        else
		        {
		        	if(in_button)
		        	{
		        		apply_surface(a, b, s1, screen);
		        		SDL_Flip( screen ); 
		        		in_button = false;
					}
				}
			}
		}
		control_fps();
	}
	return false;
}

bool Game::is_end()													// end_time < 0 时才能退出 
{
	return end;
}

void Game::create_exp()												// 创建一个式子 
{
	int a, b, c, max, min;
	ostringstream os;												// 用来构造字符串的 
	expression = "";	
	
	a = random(1, 3) < 3? random(1, 10):random(11, 20);				// 2/3 的概率 1-10，1/3的概率 11-20 

	if( random(1,2) == 1 )											// 表示接下来是加号 
	{
		b = random(1, 21-a);										// 确保 a+b <= 21 
		answer = a + b;
		os << a << "+" << b;
	}
	else															// 如果是减法，确保 a-b > -10 
	{
		if( a < 10 )
			b = random(1, 10);
		else
			b = random(1, 3) < 3? random(1, 10):random(11, 20);
		answer = a - b;
		os << a << "-" << b;
	}
	if( answer >= 10 )												// 此时接下去必须用减法 
	{
		min = answer - 9 > 20? 20: answer - 9;
		max = answer - 1 > 20? 20: answer -1;
		c = random(min, max);										// 保证 c 不会超过 20 
		os << "-" << c;
		answer -= c;
	}
	else if( answer <= 0 )											// 此时接下去必须用加法 
	{
		min = 1 - answer > 20? 20: 1 - answer;
		max = 10 - answer > 20? 20: 10 - answer;
		c = random(min, max);										// 保证 c 不会超过 20 
		os << "+" << c;
		answer += c;
	}
	else															// 如果 1 <= a+b < 10  
	{
		int t = random(1,4);
		if( t == 1 || answer == 1 )									// 1/4 的概率接下来是加法 
		{
			c = random(1, 10-answer);
			os << "+" << c;
			answer += c;
		}
		if( t == 2 )												// 1/4 的概率接下来是减法 
		{
			c = random(1, answer-1);
			os << "-" << c;
			answer -= c;
		}
																	// 1/2 的概率接下来什么都没有 
	}
	expression += os.str();
	setup_exp();
}

void Game::setup_exp()												// 将式子转换成图片 
{
	SDL_FillRect(exp_bg, NULL, 0x0);								// 清除掉原本的式子 
	for(int i = 0; i < expression.length(); i++)
	{
		if( expression[i] >= '0' && expression[i] <= '9' )
		{
			int index = expression[i] - '0';
			apply_surface(NUMBER_WIDTH*i, 0, num[index], exp_bg);	// 数字 
		}
		else if( expression[i] == '+' )
		{
			apply_surface(NUMBER_WIDTH*i, 0, plus, exp_bg);
		}
		else if( expression[i] == '-' )
		{
			apply_surface(NUMBER_WIDTH*i, 0, minus, exp_bg);
		}
	}
	int t = random(1, 8);											// 随机出现的位置 
	int exp_len =  NUMBER_WIDTH * expression.length();				// 式子的长度 
	exp_x = EXP_BEGIN + t * EXP_MARGIN - exp_len / 2;				//  
	exp_y = 0;
	reply = t + 1;
	adjust = 0;
}

void Game::move_exp(int key)										// 移动式子 
{
	int exp_len =  NUMBER_WIDTH * expression.length();				// 生成的式子图片的长度 
	switch( key )
	{
		case SDLK_LEFT: 											// 向左移动 
			reply = reply <= 1? 1: reply - 1;						// reply 最小是 1 
			exp_x = EXP_BEGIN + (reply-1) * EXP_MARGIN - exp_len / 2;
			if( exp_x < 0 )											// exp_x 最小是 0，即式子左端不能超出界面 
				exp_x = 0;
			break;
		case SDLK_RIGHT: 											// 向右移动 
			reply = reply >= 10? 10: reply + 1;						// reply 最大是 10 
			exp_x = EXP_BEGIN + (reply-1) * EXP_MARGIN - exp_len / 2;
			if( exp_x + exp_len > SCREEN_WIDTH)						// 式子右端不能超出界面 
				exp_x = SCREEN_WIDTH - exp_len;
			break;
	}
}

void Game::handle_input( const SDL_Event &event )					// 处理输入 
{
	if( event.type == SDL_KEYDOWN )									// 按下按键时 
	{
		move_exp( event.key.keysym.sym );
	}
	else if( event.type == SDL_QUIT )								// 叉掉窗口 
	{
		end = true;
	}
}

void Game::game_run()												// 游戏运行 
{
	if( exp_y >= EXP_MAX_HEIGHT )									// 如果式子碰到篮子 
	{
		if( reply == answer )										// 检查答案对不对 
		{
			score += 100;											// 加分 
			Mix_PlayChannel(-1, correct, 0);						// 播放正确音效 
			reply = 0;												// 重置玩家的回答 
		}
		else
		{ 
			Mix_PlayChannel(-1, error, 0);							// 答错，播放错误的音效 
		}
		if( game_timer.get_ticks() > GAME_TIME )					// 游戏时间到了 
		{
			end = true;
		}
		if(!end)													// 游戏没结束就继续掉落新的式子 
			create_exp();
		else
			exp_y = -1000;
	}
	
	show_game();													// 显示画面 
	control_fps();													// 控制帧率 
} 

void Game::control_fps()											// 控制游戏帧率 
{
	if( fps.get_ticks() < 1000 / FPS )								 
    {
        SDL_Delay( ( 1000 / FPS ) - fps.get_ticks() );
    }
    fps.start();
}

/***********************Game类实现结束******************************/
