#pragma once

#include <SDL.h>

class LWindow {
public:
	LWindow();
	~LWindow();
	bool init();
	SDL_Renderer* createRenderer();
	void handleEvent(SDL_Event& e);
	void setFullScreen(bool fullscreen);
	void free();
	int getWidth();
	int getHeight();
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimised();
	bool isFullscreen();

private:
	SDL_Window* mWindow;
	int mWidth;
	int mHeight;
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimised;

};