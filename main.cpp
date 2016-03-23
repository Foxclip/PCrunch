#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <sstream>
#include "ltexture.h"

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
LTexture* gPromptTextTexture;
LTexture* gTimeTextTexture;
Uint32 startTime = 0;
std::stringstream timeText;

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
	gPromptTextTexture = new LTexture(&gRenderer, &gFont);
	gTimeTextTexture = new LTexture(&gRenderer, &gFont);
	if(!gPromptTextTexture->loadFromRenderedText("Press Enter to Reset Start Time", textColor)) {
		printf("Font rendering error\n");
		return false;
	}

	return true;
}

void close() {
	gPromptTextTexture->free();
	gTimeTextTexture->free();
	delete gPromptTextTexture;
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
	timeText << "Milliseconds since start time " << SDL_GetTicks() - startTime;
	if(!gTimeTextTexture->loadFromRenderedText(timeText.str().c_str(), { 0, 0, 0 }))
		printf("Time text rendering error\n");
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);
	gPromptTextTexture->render((WIDTH - gPromptTextTexture->getWidth()) / 2, 0);
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
	case SDLK_RETURN:
		startTime = SDL_GetTicks();
	default:
		break;
	}
}