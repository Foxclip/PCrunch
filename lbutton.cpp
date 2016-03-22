#include <SDL.h>
#include "lbutton.h"

LButton::LButton() {
	mPosition.x = 0;
	mPosition.y = 0;
	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y) {
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event * e) {
	if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		if(!cursorInside(x, y)) {
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
			return;
		}
		switch(e->type) {
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;
			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
		}
	}
}

bool LButton::cursorInside(int x, int y) {
	if(x < mPosition.x)
		return false;
	if(x > mPosition.x + BUTTON_WIDTH)
		return false;
	if(y < mPosition.y)
		return false;
	if(y > mPosition.y + BUTTON_HEIGHT)
		return false;
	return true;
}

void LButton::render() {
	gButtonSpriteSheetTexture->render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}
