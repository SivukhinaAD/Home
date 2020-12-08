/*
 * main.cc
 *
 *  Created on: 25 нояб. 2020 г.
 *      Author: Anastasiya
 */
#include <iostream>
#include <vector>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <random>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_main.h>

std::minstd_rand rnd;

constexpr int WIDTH = 800, HEIGHT = 600;

using WindowPtr = std::shared_ptr<SDL_Window>;
using RendererPtr = std::shared_ptr<SDL_Renderer>;
using TexturePtr = std::shared_ptr<SDL_Texture>;


WindowPtr win;
RendererPtr ren;
TexturePtr bird;
TexturePtr dog;
TexturePtr push; //пушинки
TexturePtr oduv;//одуван

int bx = 100, by = 70, bw = 100, bh = 85;
int ba = 0, da=0, oa=0;
int ox=50, oy=450, ow=90, oh=110;
int dx=500,dy=410,dw=200,dh=150,d_dy=0;
bool d_right=true,d_jump=false;
int px=50,py=35,pw=50,ph=74;
bool push_start=false;

inline void SDL_DIE(const char * where)
{
	throw std::runtime_error(
			std::string(where) +
			std::string(": ") +
			std::string(SDL_GetError()));
}

TexturePtr load_texture(const char * filename)
{
	SDL_Surface * surf = nullptr;
	surf = IMG_Load (filename); 	// Загрузка картинки
	if (surf == nullptr) SDL_DIE("IMG_Load()");
	TexturePtr result;
	result = TexturePtr(
			SDL_CreateTextureFromSurface(ren.get(), surf),
			SDL_DestroyTexture);
	if (result == nullptr) SDL_DIE("SDL_CreateTextureFromSurface()");
	SDL_FreeSurface(surf);

	return result;
}

void render_background()
{
	SDL_SetRenderDrawColor(ren.get(),65, 105, 255, 100);
	SDL_RenderClear(ren.get());
}

void render_scene()
{
	SDL_SetRenderDrawColor(ren.get(),135,206,235,255);
	SDL_RenderClear(ren.get());

	SDL_Rect r0{0,0,800,150}; //небо
	SDL_SetRenderDrawColor(ren.get(),135,206,235,255);
	SDL_RenderFillRect(ren.get(),&r0);

	SDL_Rect r1{0,549,800,50}; //земля
	SDL_SetRenderDrawColor(ren.get(),0,128, 0,255);
	SDL_RenderFillRect(ren.get(),&r1);

	SDL_SetRenderDrawColor(ren.get(),25, 25, 112,255);//забор
	for(int j=0;j<WIDTH;j=j+5){
		SDL_RenderDrawLine(ren.get(),j,549,j+10,451);
		SDL_RenderDrawLine(ren.get(),j,549,j-10,451);
	}

	SDL_Rect r3{300,350,200,200};//основа дома
	SDL_SetRenderDrawColor(ren.get(),205,92,92,255);
	SDL_RenderFillRect(ren.get(),&r3);

	SDL_Rect r2{320,400,70,150};//дверь
	SDL_SetRenderDrawColor(ren.get(),139,69,19,255);
	SDL_RenderFillRect(ren.get(),&r2);

	SDL_Rect r4{410,400,80,80};//рама окна
	SDL_SetRenderDrawColor(ren.get(),139,69,19,255);
	SDL_RenderFillRect(ren.get(),&r4);

	SDL_Rect r5{415,405,30,70};//окно1
	SDL_SetRenderDrawColor(ren.get(),255, 255, 224,255);
	SDL_RenderFillRect(ren.get(),&r5);

	SDL_Rect r6{455,405,30,70};//окно2
	SDL_SetRenderDrawColor(ren.get(),255, 255, 224,255);
	SDL_RenderFillRect(ren.get(),&r6);

	SDL_Rect r7{455,230,30,90};//труба
	SDL_SetRenderDrawColor(ren.get(),165,42,42,255);
	SDL_RenderFillRect(ren.get(),&r7);

	SDL_Rect r8{375,460,8,20};//ручка дверная
	SDL_SetRenderDrawColor(ren.get(),105,105,105,255);
	SDL_RenderFillRect(ren.get(),&r8);

	SDL_Rect r12{220,250,80,300};//II часть дома
		SDL_SetRenderDrawColor(ren.get(),205,92,92,255);
		SDL_RenderFillRect(ren.get(),&r12);

	SDL_Rect r11{298,350,2,200};//полоска между частями дома
		SDL_SetRenderDrawColor(ren.get(),128, 128, 128,255);//240, 128, 128
		SDL_RenderFillRect(ren.get(),&r11);

	SDL_Rect r13{230,270,60,80};//рама окна
		SDL_SetRenderDrawColor(ren.get(),139,69,19,255);
		SDL_RenderFillRect(ren.get(),&r13);

	SDL_Rect r14{235,275,50,70};//окно3
		SDL_SetRenderDrawColor(ren.get(),255, 255, 224,255);
		SDL_RenderFillRect(ren.get(),&r14);

	SDL_SetRenderDrawColor(ren.get(), 128,0,0,255);//крыша пристроя
			for(int i=0;i<50;i++){
				SDL_RenderDrawLine(ren.get(),210+i,250-i,310-i,250-i);
			}

	SDL_SetRenderDrawColor(ren.get(), 128,0,0,255);//крыша
		for(int i=0;i<121;i++){
			SDL_RenderDrawLine(ren.get(),280+i,350-i,520-i,350-i);
		}

	SDL_Rect r9{520,450,100,100};//основа будки
		SDL_SetRenderDrawColor(ren.get(),160, 82, 45,255);
		SDL_RenderFillRect(ren.get(),&r9);

	SDL_Rect r10{540,460,60,80};//дверь будки
		SDL_SetRenderDrawColor(ren.get(),139, 69, 19,255);
		SDL_RenderFillRect(ren.get(),&r10);

	SDL_SetRenderDrawColor(ren.get(),139, 69, 19,255);//крыша будки
			for(int i=0;i<55;i++){
				SDL_RenderDrawLine(ren.get(),515+i,450-i,625-i,450-i);
			}

	SDL_Rect r_oduv {ox, oy, ow, oh};//одуванчик
	SDL_Point p_oduv_center { ow/2, oh/2 };
	SDL_RenderCopyEx(
			ren.get(), oduv.get(),
			nullptr, &r_oduv,
			oa,						// Угол поворота
			&p_oduv_center,			// Центр поворота (с.к. текстуры)
			SDL_FLIP_NONE);

	SDL_Rect r_bird { bx, by, bw, bh }; //птица
	SDL_Point p_bird_center { bw/2, bh/2 };
	SDL_RenderCopyEx(
		ren.get(), bird.get(),
		nullptr, &r_bird,
		ba,						// Угол поворота
		&p_bird_center,			// Центр поворота (с.к. текстуры)
		SDL_FLIP_NONE);

	SDL_Rect r_dog { dx, dy, dw, dh }; //собака
	auto flip=d_right?SDL_FLIP_NONE:SDL_FLIP_HORIZONTAL;
	SDL_Point p_dog_center { dw/2, dh/2 };
	SDL_RenderCopyEx(
		ren.get(), dog.get(),
		nullptr, &r_dog,
		da,						// Угол поворота
		&p_dog_center,			// Центр поворота (с.к. текстуры)
		flip);

	if(push_start){
		SDL_Rect r_push { px, py, pw, ph }; //пушинки
		SDL_Point p_push_center { pw/2, ph/2 };
		SDL_RenderCopyEx(
				ren.get(), push.get(),
				nullptr, &r_push,
				0.,						// Угол поворота
				&p_push_center,			// Центр поворота (с.к. текстуры)
				SDL_FLIP_NONE);
	}

	SDL_RenderPresent(ren.get());
}

void main_loop()
{
	SDL_Event event;
	auto keys = SDL_GetKeyboardState(nullptr);
	for(;;) {
		// Обработка событий
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: return;
			default: ;
			}
		}
		// Изменение состояния
		bx=bx+4;
		if (keys[SDL_SCANCODE_LEFT]) bx -= 4;
		if (keys[SDL_SCANCODE_RIGHT]) bx += 4;
		if (keys[SDL_SCANCODE_UP]) by -= 4;
		if (keys[SDL_SCANCODE_DOWN]) by += 4;

		if(by<0) by=0;
		if(by>=70)by=69;
		if(bx>=WIDTH)bx=-64;

		if(keys[SDL_SCANCODE_X]){
			dx=dx+6;
			d_right=true;
		}
		if(keys[SDL_SCANCODE_Z]){
			dx=dx-6;
			d_right=false;
		}
		if(dx>=WIDTH-150)dx=WIDTH-150;
		if(dx<0)dx=0;

		if(keys[SDL_SCANCODE_S] && !d_jump){
			d_jump=true;
			d_dy=-16;
		}
		if(d_jump){
			dy=dy+d_dy;
			d_dy=d_dy+1;
			if(dy>400){
				dy=400;
				d_jump=false;
			}
		}

		if(keys[SDL_SCANCODE_SPACE] && !push_start) {
			push_start=true;
			px=ox+5;
			py=oy-85;
		}
		if(push_start==true) py=py-4;
		if(py>=HEIGHT) push_start=false;

		// Рисование
		render_background();
		render_scene();
		SDL_RenderPresent(ren.get());
	}
}

int main(int,  char**){
	try {
			if (SDL_Init(SDL_INIT_EVERYTHING) != 0) SDL_DIE("SDL_Init()");

			win = WindowPtr(
					SDL_CreateWindow(
							"SDL window",
							SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							WIDTH, HEIGHT,
							SDL_WINDOW_SHOWN),
					SDL_DestroyWindow);
			if (win == nullptr) SDL_DIE("SDL_CreateWindow()");

			ren = RendererPtr(
					SDL_CreateRenderer(win.get(), -1,
							SDL_RENDERER_ACCELERATED |
							SDL_RENDERER_PRESENTVSYNC),
					SDL_DestroyRenderer);
			if (ren == nullptr) SDL_DIE("SDL_CreateRenderer()");

			// Тут будет загрузка всяких ресурсов
			bird = load_texture("bird.png");
			dog = load_texture("dog.png");
			push = load_texture("push.png");
			oduv = load_texture("odu.png");

			main_loop();


		} catch (const std::exception& e) {
			std::cerr << "Произошла ошибка:\n" <<
					e.what() << std::endl;
			return 1;
		} catch (...) {
			std::cerr << "Произошла какая-то ошибка." << std::endl;
			return 2;
		}

		return 0;
}
