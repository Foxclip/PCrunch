#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include "ltexture.h"

bool init();
bool loadMedia();
void close();
void mainLoop();
void render();
void processInput();
void processKeyboard();

const int WIDTH = 640;
const int HEIGHT = 480;
bool gQuit = false;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture** currentTexture;
LTexture *upTexture, *downTexture, *rightTexture, *leftTexture, *pressTexture;

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
	/*if(TTF_Init() == -1) {
		printf("TTF init error: %s\n", TTF_GetError());
		return false;
	}*/
	return true;
}

bool loadMedia() {
	/*gFont = TTF_OpenFont("lazy.ttf", 28);
	if(!gFont) {
		printf("Font loading error: %s\n", TTF_GetError());
		return false;
	}*/
	SDL_Color textColor = { 0, 0, 0 };
	upTexture = new LTexture(&gRenderer);
	if(!upTexture->loadFromFile("up.png"))
		return false;
	downTexture = new LTexture(&gRenderer);
	if(!downTexture->loadFromFile("down.png"))
		return false;
	leftTexture = new LTexture(&gRenderer);
	if(!leftTexture->loadFromFile("left.png"))
		return false;
	rightTexture = new LTexture(&gRenderer);
	if(!rightTexture->loadFromFile("right.png"))
		return false;
	pressTexture = new LTexture(&gRenderer);
	if(!pressTexture->loadFromFile("press.png"))
		return false;

	return true;
}

void close() {
	upTexture->free();
	downTexture->free();
	leftTexture->free();
	rightTexture->free();
	pressTexture->free();
	delete upTexture;
	delete downTexture;
	delete leftTexture;
	delete rightTexture;
	delete pressTexture;
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	IMG_Quit();
	SDL_Quit();
}

void mainLoop() {
	while(!gQuit) {
		processInput();
		processKeyboard();
		render();
	}
}

void render() {
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);
	(*currentTexture)->render(0, 0);
	SDL_RenderPresent(gRenderer);
}

void processInput() {
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0) {
		if(e.type == SDL_QUIT)
			gQuit = true;
	}
}

void processKeyboard() {
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if(currentKeyStates[SDL_SCANCODE_UP])
		currentTexture = &upTexture;
	else if(currentKeyStates[SDL_SCANCODE_DOWN])
		currentTexture = &downTexture;
	else if(currentKeyStates[SDL_SCANCODE_LEFT])
		currentTexture = &leftTexture;
	else if(currentKeyStates[SDL_SCANCODE_RIGHT])
		currentTexture = &rightTexture;
	else
		currentTexture = &pressTexture;
}