#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include "ltexture.h"
#include "lbutton.h"

bool init();
bool loadMedia();
void close();
void mainLoop();
void render();
void processInput();
void processKeyboard(SDL_Event e);

const int WIDTH = 640;
const int HEIGHT = 480;
bool gQuit = false;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
LButton* gButtons[TOTAL_BUTTONS];
LTexture* gButtonSpriteSheetTexture;
SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

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
	SDL_Color textColor = { 0, 0, 0 };
	gButtonSpriteSheetTexture = new LTexture(&gRenderer);
	if(!gButtonSpriteSheetTexture->loadFromFile("button.png"))
		return false;

	gSpriteClips[0] = { 0, 0, 300, 200 };
	gSpriteClips[1] = { 0, 200, 300, 200 };
	gSpriteClips[2] = { 0, 400, 300, 200 };
	gSpriteClips[3] = { 0, 600, 300, 200 };

	for(int i = 0; i < TOTAL_BUTTONS; i++)
		gButtons[i] = new LButton();

	gButtons[0]->setPosition(0, 0);
	gButtons[1]->setPosition(WIDTH - 300, 0);
	gButtons[2]->setPosition(0, HEIGHT - 200);
	gButtons[3]->setPosition(WIDTH - 300, HEIGHT - 200);

	return true;
}

void close() {
	gButtonSpriteSheetTexture->free();
	for(int i = 0; i < TOTAL_BUTTONS; i++)
		delete gButtons[i];
	delete gButtonSpriteSheetTexture;
	TTF_CloseFont(gFont);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gFont = NULL;
	gWindow = NULL;
	gRenderer = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void mainLoop() {
	while(!gQuit) {
		processInput();
		render();
	}
}

void render() {
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);
	for(int i = 0; i < TOTAL_BUTTONS; i++)
		gButtons[i]->render();
	SDL_RenderPresent(gRenderer);
}

void processInput() {
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0) {
		if(e.type == SDL_QUIT)
			gQuit = true;
		else if(e.type == SDL_KEYDOWN)
			processKeyboard(e);
		else
			for(int i = 0; i < TOTAL_BUTTONS; i++)
				gButtons[i]->handleEvent(&e);
	}
}

void processKeyboard(SDL_Event e) {
	switch(e.key.keysym.sym) {
		case SDLK_ESCAPE:
			gQuit = true;
			break;
		default:
			break;
	}
}