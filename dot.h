#pragma once

#include <SDL.h>

class Dot {
public:
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	static const int DOT_VEL = 10;
	Dot();
	void handleEvent(SDL_Event& e);
	void move();
	void render();

private:
	int mPosX, mPosY;
	int mVelX, mVelY;

};