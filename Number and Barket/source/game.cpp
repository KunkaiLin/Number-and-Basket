/*******************************************************************
���ļ�������
	1. ȫ�ֺ�����ʵ�֣� 
	2. ���õ���ľ���ʵ��. 
ʱ�䣺2018/11/8
********************************************************************/

#include "game.h"

SDL_Surface* screen;

/***********************ȫ�ֺ���ʵ�ֿ�ʼ****************************/

SDL_Surface* load_image( string filename )							// ����һ��ͼƬ 
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
																	// �� source ���� destination 
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination , SDL_Rect *clip )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init_SDL()														// ��ʼ�� SDL ���䲿�� 
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
    
    SDL_WM_SetCaption( "Number and Basket", NULL );					// ���ñ��� 
	SDL_WM_SetIcon(SDL_LoadBMP(string(pic_path + "icon.bmp").c_str()), NULL);				// ����ͼ�� 
	
    return true;
}

bool quit_SDL()														// �˳� SDL 
{
	Mix_CloseAudio();
	
	SDL_Quit();
}

int random( int min, int max )										// ���� min~max ֮���һ������� 
{
	if( min > max )
		return 0;
	return rand() % ( max - min + 1 ) + min;
}

/***********************ȫ�ֺ���ʵ�ֽ���****************************/

/***********************Timer��ʵ�ֿ�ʼ*****************************/ 

Timer::Timer()														// ����
{
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}
 
void Timer::start()													// ��ʼ��ʱ
{
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();									// ��õ�ǰʱ�� 
}

void Timer::stop()													// ֹͣ��ʱ
{
	started = false;
	paused = false;
}

int Timer::get_ticks()												// ��ü�ʱ��ʼ�����ڹ��˶೤ʱ�� 
{
	if( started == true )											// ���ѿ�ʼ��ʱ 
	{
		if( paused == true )										// ����ͣ��������ͣʱ��ʱ�� 
		{
			return pausedTicks;
		}
		else
		{
			return SDL_GetTicks() - startTicks;						// ���򷵻ص�ǰʱ�俪ʼʱʱ�� 
		}
	}
	
	return 0;
}

void Timer::pause()													// ��ͣ��ʱ 					
{
	if( ( started == true ) && ( paused == false ) )
	{
		paused = true;
		
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

void Timer::unpause()												// �ָ���ʱ 
{
	if( paused == true )
	{
		paused = false;
		
		startTicks = SDL_GetTicks() - pausedTicks;
		
		pausedTicks = 0;
	}
}

bool Timer::is_started()											// �Ƿ�ʼ 
{
    return started;    
}
 
bool Timer::is_paused()												// �Ƿ���ͣ 
{
    return paused;    
}
/***********************Time��ʵ�ֽ���******************************/

/***********************Game��ʵ�ֿ�ʼ******************************/

Game::Game()														// ���캯������Ҫ��������һЩ��Դ 
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
	Mix_PlayMusic( game_music, -1 );								// ���ű������� 
	fps.start();													// ��ʼ��ʱ 
}

Game::~Game()														// �����������ͷ���Դ 
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


void Game::reset_data()												// ����һЩ���� 
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

bool Game::show_begin()												// ��ʾ��ʼ���� 
{
	apply_surface(0, 0, background, screen);						// ���� 
	apply_surface((SCREEN_WIDTH-author->w) / 2, 90, author, screen);
	apply_surface(START_X, START_Y, play[0], screen);				// ��ʼ��Ϸ��ť 
	SDL_Flip( screen );												// ˢ����Ļ 

	if( wait_click(play[0], play[1], START_X, START_Y) )
	{
		show_game();												// ��ʾ��Ϸ�Ļ��� 
		for(int i = 2; i >= 0; i--)									// ��ʾ����ʱ 
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

void Game::show_game()												// ��ʾ��Ϸ���� 
{		
	apply_surface(0, 0, background, screen);						// ���� 

	if( exp_bg )													// ��ʾʽ�� 
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
	
	for(int i = 0; i < NUMBER_NUM; i++)								// ��ʾ���� 
	{
		apply_surface(5+i*120, SCREEN_HEIGHT-74, basket[i], screen);
	}
																	// ��ʾʣ��ʱ�� 
	ostringstream os;
	double left_time = ((double)GAME_TIME - game_timer.get_ticks())/1000;
	if( left_time < 0 )
		left_time = 0;
	os <<"Time: " << setiosflags(ios::fixed)<<setprecision(2) << left_time << "s";
	string t = os.str();
	SDL_Surface *message = TTF_RenderText_Solid( font, t.c_str(), TIME_COLOR );
	apply_surface( 950, 30, message, screen );
																	// ��ʾ���� 
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
	bool in_button = false;											// ��ʾ����Ƿ��ڰ�ť�� 
	while( true )													// ��û�н�����Ϸ 
	{
		if( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_QUIT )							// ���������
			{ 
				return false;
			}
			if( event.type == SDL_MOUSEBUTTONDOWN )					// ������ 
			{
				if( event.button.button == SDL_BUTTON_LEFT )		// ������ 
				{
					int x = event.button.x;							// ���λ�õ� x��y ���� 
					int y = event.button.y;							
					if( x >= a && x <= a + s1->w && y >= b && y <= b + s1->h )
					{
						return true;
					}
				}
			}
			if( event.type == SDL_MOUSEMOTION )						// ����ƶ�����ʼ��ť�ڣ���ť��ɫ 
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

bool Game::is_end()													// end_time < 0 ʱ�����˳� 
{
	return end;
}

void Game::create_exp()												// ����һ��ʽ�� 
{
	int a, b, c, max, min;
	ostringstream os;												// ���������ַ����� 
	expression = "";	
	
	a = random(1, 3) < 3? random(1, 10):random(11, 20);				// 2/3 �ĸ��� 1-10��1/3�ĸ��� 11-20 

	if( random(1,2) == 1 )											// ��ʾ�������ǼӺ� 
	{
		b = random(1, 21-a);										// ȷ�� a+b <= 21 
		answer = a + b;
		os << a << "+" << b;
	}
	else															// ����Ǽ�����ȷ�� a-b > -10 
	{
		if( a < 10 )
			b = random(1, 10);
		else
			b = random(1, 3) < 3? random(1, 10):random(11, 20);
		answer = a - b;
		os << a << "-" << b;
	}
	if( answer >= 10 )												// ��ʱ����ȥ�����ü��� 
	{
		min = answer - 9 > 20? 20: answer - 9;
		max = answer - 1 > 20? 20: answer -1;
		c = random(min, max);										// ��֤ c ���ᳬ�� 20 
		os << "-" << c;
		answer -= c;
	}
	else if( answer <= 0 )											// ��ʱ����ȥ�����üӷ� 
	{
		min = 1 - answer > 20? 20: 1 - answer;
		max = 10 - answer > 20? 20: 10 - answer;
		c = random(min, max);										// ��֤ c ���ᳬ�� 20 
		os << "+" << c;
		answer += c;
	}
	else															// ��� 1 <= a+b < 10  
	{
		int t = random(1,4);
		if( t == 1 || answer == 1 )									// 1/4 �ĸ��ʽ������Ǽӷ� 
		{
			c = random(1, 10-answer);
			os << "+" << c;
			answer += c;
		}
		if( t == 2 )												// 1/4 �ĸ��ʽ������Ǽ��� 
		{
			c = random(1, answer-1);
			os << "-" << c;
			answer -= c;
		}
																	// 1/2 �ĸ��ʽ�����ʲô��û�� 
	}
	expression += os.str();
	setup_exp();
}

void Game::setup_exp()												// ��ʽ��ת����ͼƬ 
{
	SDL_FillRect(exp_bg, NULL, 0x0);								// �����ԭ����ʽ�� 
	for(int i = 0; i < expression.length(); i++)
	{
		if( expression[i] >= '0' && expression[i] <= '9' )
		{
			int index = expression[i] - '0';
			apply_surface(NUMBER_WIDTH*i, 0, num[index], exp_bg);	// ���� 
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
	int t = random(1, 8);											// ������ֵ�λ�� 
	int exp_len =  NUMBER_WIDTH * expression.length();				// ʽ�ӵĳ��� 
	exp_x = EXP_BEGIN + t * EXP_MARGIN - exp_len / 2;				//  
	exp_y = 0;
	reply = t + 1;
	adjust = 0;
}

void Game::move_exp(int key)										// �ƶ�ʽ�� 
{
	int exp_len =  NUMBER_WIDTH * expression.length();				// ���ɵ�ʽ��ͼƬ�ĳ��� 
	switch( key )
	{
		case SDLK_LEFT: 											// �����ƶ� 
			reply = reply <= 1? 1: reply - 1;						// reply ��С�� 1 
			exp_x = EXP_BEGIN + (reply-1) * EXP_MARGIN - exp_len / 2;
			if( exp_x < 0 )											// exp_x ��С�� 0����ʽ����˲��ܳ������� 
				exp_x = 0;
			break;
		case SDLK_RIGHT: 											// �����ƶ� 
			reply = reply >= 10? 10: reply + 1;						// reply ����� 10 
			exp_x = EXP_BEGIN + (reply-1) * EXP_MARGIN - exp_len / 2;
			if( exp_x + exp_len > SCREEN_WIDTH)						// ʽ���Ҷ˲��ܳ������� 
				exp_x = SCREEN_WIDTH - exp_len;
			break;
	}
}

void Game::handle_input( const SDL_Event &event )					// �������� 
{
	if( event.type == SDL_KEYDOWN )									// ���°���ʱ 
	{
		move_exp( event.key.keysym.sym );
	}
	else if( event.type == SDL_QUIT )								// ������� 
	{
		end = true;
	}
}

void Game::game_run()												// ��Ϸ���� 
{
	if( exp_y >= EXP_MAX_HEIGHT )									// ���ʽ���������� 
	{
		if( reply == answer )										// ���𰸶Բ��� 
		{
			score += 100;											// �ӷ� 
			Mix_PlayChannel(-1, correct, 0);						// ������ȷ��Ч 
			reply = 0;												// ������ҵĻش� 
		}
		else
		{ 
			Mix_PlayChannel(-1, error, 0);							// ������Ŵ������Ч 
		}
		if( game_timer.get_ticks() > GAME_TIME )					// ��Ϸʱ�䵽�� 
		{
			end = true;
		}
		if(!end)													// ��Ϸû�����ͼ��������µ�ʽ�� 
			create_exp();
		else
			exp_y = -1000;
	}
	
	show_game();													// ��ʾ���� 
	control_fps();													// ����֡�� 
} 

void Game::control_fps()											// ������Ϸ֡�� 
{
	if( fps.get_ticks() < 1000 / FPS )								 
    {
        SDL_Delay( ( 1000 / FPS ) - fps.get_ticks() );
    }
    fps.start();
}

/***********************Game��ʵ�ֽ���******************************/
