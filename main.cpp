#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <sstream>
#include "ltexture.h"
#include "ltimer.h"

bool init();
bool loadMedia();
void close();
void mainLoop();
void render();
void handleEvents();
void handleKeyboard(SDL_Event e);

const int WIDTH = 640;
const int HEIGHT = 480;
bool gQuit = false;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont;
LTexture* gStartPromptTextTexture;
LTexture* gPausePromptTextTexture;
LTexture* gTimeTextTexture;
std::stringstream timeText;
LTimer timer;

int main(int argc, char* args[]) {
	if(!init()) {
		printf("Failed to init\n");
	} else if(!loadMedia()) {
		printf("Failed to load media\n");
	} else {
		mainLoop();
	}
	close();
	return 0;
}

bool init() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Init error: %s\n", SDL_GetError());
		return false;
	}
	gWindow = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if(gWindow == NULL) {
		printf("Window creation error: %s\n", SDL_GetError());
		return false;
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
	SDL_Color textColor = { 0, 0, 0, 255 };
	gStartPromptTextTexture = new LTexture(&gRenderer, &gFont);
	gPausePromptTextTexture = new LTexture(&gRenderer, &gFont);
	gTimeTextTexture = new LTexture(&gRenderer, &gFont);
	if(!gStartPromptTextTexture->loadFromRenderedText("Press S to Start or Stop the Timer", textColor)) {
		printf("Font rendering error\n");
		return false;
	}
	if(!gPausePromptTextTexture->loadFromRenderedText("Press P to Pause or Unpause the timer", textColor)) {
		printf("Font rendering error\n");
		return false;
	}

	return true;
}

void close() {
	gStartPromptTextTexture->free();
	gPausePromptTextTexture->free();
	gTimeTextTexture->free();
	delete gStartPromptTextTexture;
	delete gPausePromptTextTexture;
	delete gTimeTextTexture;
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	gFont = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void mainLoop() {
	while(!gQuit) {
		handleEvents();
		render();
	}
}

void render() {
	timeText.str("");
	timeText << "Seconds since start time " << timer.getTicks() / 1000.0;
	if(!gTimeTextTexture->loadFromRenderedText(timeText.str().c_str(), { 0, 0, 0 }))
		printf("Time text rendering error\n");
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);
	gStartPromptTextTexture->render((WIDTH - gStartPromptTextTexture->getWidth()) / 2, 0);
	gPausePromptTextTexture->render((WIDTH - gPausePromptTextTexture->getWidth()) / 2, gStartPromptTextTexture->getHeight());
	gTimeTextTexture->render((WIDTH - gTimeTextTexture->getWidth()) / 2, (HEIGHT - gTimeTextTexture->getHeight()) / 2);
	SDL_RenderPresent(gRenderer);
}

void handleEvents() {
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0) {
		if(e.type == SDL_QUIT)
			gQuit = true;
		else if(e.type == SDL_KEYDOWN)
			handleKeyboard(e);
	}
}

void handleKeyboard(SDL_Event e) {
	switch(e.key.keysym.sym) {
	case SDLK_s:
		timer.isStarted() ? timer.stop() : timer.start();
		break;
	case SDLK_p:
		timer.isPaused() ? timer.unpause() : timer.pause();
	default:
		break;
	}
}