#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <sstream>
#include "lwindow.h"
#include "ltexture.h"
#include "globals.h"

bool init();
bool loadMedia();
void close();
void mainLoop();
void render();
void handleEvents();
void handleKeyboard(SDL_Event e);
void processPhysics();

bool gQuit = false;
LWindow gWindow;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont;
LTexture gSceneTexture;

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
	gSceneTexture = LTexture(&gRenderer);
	gSceneTexture.loadFromFile("window.png");
	return true;
}

void close() {
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();
	gRenderer = NULL;
	gFont = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void mainLoop() {
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
	gSceneTexture.render((gWindow.getWidth() - gSceneTexture.getWidth()) / 2, (gWindow.getHeight() - gSceneTexture.getHeight()) / 2);
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
	switch(e.key.keysym.sym) {
	case SDLK_RETURN:
		gWindow.isFullscreen() ? gWindow.setFullScreen(false) : gWindow.setFullScreen(true); break;
	}
}

void processPhysics() {
}