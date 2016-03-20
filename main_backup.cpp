#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

using namespace std;

enum Textures {
	TEXTURE_DEFAULT,
	TEXTURE_UP,
	TEXTURE_DOWN,
	TOTAL_NUMBER_OF_TEXTURES
};
const int WIDTH = 640;
const int HEIGHT = 480;
bool gQuit = false;
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Texture* gCurrentTexture = NULL;
SDL_Texture* gTextures[TOTAL_NUMBER_OF_TEXTURES];
SDL_Renderer* gRenderer = NULL;

bool init();
bool loadMedia();
void close();
void mainLoop();
SDL_Texture* loadTexture(string path);
void render();
void processInput();
void processKeyboard(SDL_Event e);

int main(int argc, char* args[]) {
	if(!init()) {
		printf("Failed to init\n");
	} else {
		if(!loadMedia()) {
			printf("Failed to load media\n");
		} else {
			mainLoop();
		}
	}
	close();
	return 0;
}

bool init() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Init error: %s\n", SDL_GetError);
		return false;
	}
	gWindow = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if(gWindow == NULL) {
		printf("Window creation error: %s\n", SDL_GetError);
		return false;
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if(gRenderer == NULL) {
		printf("Renderer creation error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	int imgFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL image error: %s\n", IMG_GetError());
		return false;
	}
	gScreenSurface = SDL_GetWindowSurface(gWindow);
	return true;
}

bool loadMedia() {
	gTextures[TEXTURE_DEFAULT] = loadTexture("default_scaled.png");
	if(gTextures[TEXTURE_DEFAULT] == NULL) {
		return false;
	}
	gTextures[TEXTURE_UP] = loadTexture("up_scaled.png");
	if(gTextures[TEXTURE_UP] == NULL) {
		return false;
	}
	gTextures[TEXTURE_DOWN] = loadTexture("down_scaled.png");
	if(gTextures[TEXTURE_DOWN] == NULL) {
		return false;
	}
	return true;
}

SDL_Texture* loadTexture(string path) {
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL) {
		printf("Image (%s) loading error: %s\n", path.c_str(), IMG_GetError());	
		return NULL;
	}
	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	if(optimizedSurface == NULL) {
		printf("Texture creation error: %s SDL error: %s\n", path.c_str(), SDL_GetError());
		return NULL;
	}
	SDL_FreeSurface(loadedSurface);
	return optimizedSurface;
}

void close() {
	for(int i = 0; i < TOTAL_NUMBER_OF_TEXTURES; i++) {
		SDL_DestroyTexture(gTextures[i]);
		gTextures[i] = NULL;
	}
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}

void mainLoop() {
	gCurrentTexture = gTextures[TEXTURE_DEFAULT];
	while(!gQuit) {
		processInput();
		render();
	}
}

void render() {
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = WIDTH/2;
	rect.h = HEIGHT/2;
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, gCurrentTexture, NULL, &rect);
	SDL_RenderPresent(gRenderer);
}

void processInput() {
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0) {
		if(e.type == SDL_QUIT)
			gQuit = true;
		else if(e.type == SDL_KEYDOWN) {
			processKeyboard(e);
		}
	}
}

void processKeyboard(SDL_Event e) {
	switch(e.key.keysym.sym) {
		case SDLK_UP:
			gCurrentTexture = gTextures[TEXTURE_UP];
			printf("Up pressed\n");
			break;
		case SDLK_DOWN:
			gCurrentTexture = gTextures[TEXTURE_DOWN];
			printf("Down pressed\n");
			break;
		default:
			printf("Something else pressed\n");
			gCurrentTexture = gTextures[TEXTURE_DEFAULT];
			break;
	}
}