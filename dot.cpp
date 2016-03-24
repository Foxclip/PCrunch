#include "dot.h"
#include "globals.h"

Dot::Dot() {
	mPosX = 100;
	mPosY = 100;
	mVelX = 0;
	mVelY = 0;
}

void Dot::handleEvent(SDL_Event& e) {
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch(e.key.keysym.sym) {
		case SDLK_UP: mVelY -= DOT_VEL; break;
		case SDLK_DOWN: mVelY += DOT_VEL; break;
		case SDLK_LEFT: mVelX -= DOT_VEL; break;
		case SDLK_RIGHT: mVelX += DOT_VEL; break;
		}
	} else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch(e.key.keysym.sym) {
		case SDLK_UP: mVelY += DOT_VEL; break;
		case SDLK_DOWN: mVelY -= DOT_VEL; break;
		case SDLK_LEFT: mVelX += DOT_VEL; break;
		case SDLK_RIGHT: mVelX -= DOT_VEL; break;
		}
	}
}

void Dot::move() {
	mPosX += mVelX;
	mPosY += mVelY;
	if(mPosX < 0) {
		mPosX = 0;
		mVelX = -mVelX;
	}
	if(mPosX > SCREEN_WIDTH - DOT_WIDTH) {
		mPosX = SCREEN_WIDTH - DOT_WIDTH;
		mVelX = -mVelX;
	}
	if(mPosY < 0) {
		mPosY = 0;
		mVelY = -mVelY;
	}
	if(mPosY > SCREEN_HEIGHT - DOT_HEIGHT) {
		mPosY = SCREEN_HEIGHT - DOT_HEIGHT;
		mVelY = -mVelY;
	}
}

void Dot::render() {
	gDotTexture.render(mPosX, mPosY);
}