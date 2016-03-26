#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_thread.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <sstream>
#include "lwindow.h"
#include "ltexture.h"
#include "globals.h"

bool initSDL();
bool loadMedia();
void close();
void init();
void mainLoop();
void render();
void handleEvents();
void handleKeyboard(SDL_Event e);
void processPhysics();
int producer(void* data);
int consumer(void* data);
void produce();
void consume();

bool gQuit = false;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont;
LWindow gWindow;
LTexture gTargetTexture;
double angle = 0;
SDL_Point screenCenter = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
SDL_Thread* thread;
SDL_mutex* gBufferLock = NULL;
SDL_cond* gCanProduce = NULL;
SDL_cond* gCanConsume = NULL;
int gData = -1;

int main(int argc, char* args[]) {
	if(!initSDL()) {
		printf("Failed to init\n");
	} else if(!loadMedia()) {
		printf("Failed to load media\n");
	} else {

		mainLoop();
	}
	close();
	return 0;
}

bool initSDL() {
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		printf("Init error: %s\n", SDL_GetError());
		return false;
	}
	if(!gWindow.init()) {
		printf("Window creation error: %s\n", SDL_GetError());
		return false;
	}
	gRenderer = gWindow.createRenderer();
	if(gRenderer == NULL) {
		printf("Renderer creation error: %s\n", SDL_GetError());
		return false;
	}
	int imgFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL image init error: %s\n", IMG_GetError());
		return false;
	}
	if(TTF_Init() == -1) {
		printf("TTF init error: %s\n", TTF_GetError());
		return false;
	}
	return true;
}

bool loadMedia() {
	gFont = TTF_OpenFont("lazy.ttf", 28);
	if(!gFont) {
		printf("Font loading error: %s\n", TTF_GetError());
		return false;
	}
	gTargetTexture = LTexture();
	gTargetTexture.createBlank(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_TEXTUREACCESS_TARGET);
	gBufferLock = SDL_CreateMutex();
	gCanProduce = SDL_CreateCond();
	gCanConsume = SDL_CreateCond();
	return true;
}

void close() {
	SDL_DestroyMutex(gBufferLock);
	gBufferLock = NULL;
	SDL_DestroyCond(gCanProduce);
	SDL_DestroyCond(gCanConsume);
	gCanProduce = NULL;
	gCanConsume = NULL;
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	gFont = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void init() {
	//srand(SDL_GetTicks());
	SDL_Thread* producerThread = SDL_CreateThread(producer, "Producer", NULL);
	SDL_Thread* consumerThread = SDL_CreateThread(consumer, "Consumer", NULL);
}

void mainLoop() {
	init();
	while(!gQuit) {
		handleEvents();
		processPhysics();
		render();
	}
}

void render() {
	if(gWindow.isMinimised())
		return;
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);
	gTargetTexture.setAsRenderTarget();
	SDL_RenderClear(gRenderer);
	SDL_Rect rect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
	SDL_RenderFillRect(gRenderer, &rect);
	SDL_SetRenderTarget(gRenderer, NULL);
	gTargetTexture.render(0, 0, NULL, angle, &screenCenter);
	SDL_RenderPresent(gRenderer);
}

void handleEvents() {
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0) {
		if(e.type == SDL_QUIT)
			gQuit = true;
		else if(e.type == SDL_KEYDOWN)
			handleKeyboard(e);
		else
			gWindow.handleEvent(e);
	}
}

void handleKeyboard(SDL_Event e) {
	/*switch(e.key.keysym.sym) {
	}*/
}

void processPhysics() {
	//angle += 2;
	if(angle > 360)
		angle = fmod(angle, 360);
}

int producer(void* data) {
	printf("Producer started\n");
	srand(SDL_GetTicks());
	for(int i = 0; i < 5; i++) {
		SDL_Delay(rand() % 1000);
		produce();
	}
	printf("Producer finished\n");
	return 0;
}

int consumer(void* data) {
	printf("Consumer started\n");
	srand(SDL_GetTicks());
	for(int i = 0; i < 5; i++) {
		SDL_Delay(rand() % 1000);
		consume();
	}
	printf("Consumer finished\n");
	return 0;
}

void produce() {
	SDL_LockMutex(gBufferLock);
	if(gData != -1) {
		printf("Producer: full buffer\n");
		SDL_CondWait(gCanProduce, gBufferLock);
	}
	gData = rand() % 255;
	printf("Producer: produced %d\n", gData);
	SDL_UnlockMutex(gBufferLock);
	SDL_CondSignal(gCanConsume);
}

void consume() {
	SDL_LockMutex(gBufferLock);
	if(gData == -1) {
		printf("Consumer: empty buffer\n");
		SDL_CondWait(gCanConsume, gBufferLock);
	}
	printf("Consumer: consumed %d\n", gData);
	gData = -1;
	SDL_UnlockMutex(gBufferLock);
	SDL_CondSignal(gCanProduce);
}