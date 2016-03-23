#pragma once

#include "ltexture.h"

enum LButtonSprite {
	BUTTON_SPRITE_MOUSE_OUT,
	BUTTON_SPRITE_MOUSE_OVER_MOTION,
	BUTTON_SPRITE_MOUSE_DOWN,
	BUTTON_SPRITE_MOUSE_UP,
	BUTTON_SPRITE_TOTAL
};

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;
extern LTexture* gButtonSpriteSheetTexture;
extern SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];

class LButton {
	public:
		LButton();
		void setPosition(int x, int y);
		void handleEvent(SDL_Event* e);
		bool cursorInside(int x, int y);
		void render();

	private:
		SDL_Point mPosition;
		LButtonSprite mCurrentSprite;

};