#pragma once

#include <SDL_ttf.h>
#include <string>

class LTexture {
	public:
		LTexture();
		LTexture(SDL_Renderer** renderer);
		#ifdef _SDL_TTF_H
		LTexture(SDL_Renderer** renderer, TTF_Font** font);
		#endif
		~LTexture();
		bool loadFromFile(std::string path);
		#ifdef _SDL_TTF_H
		bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
		#endif
		void free();
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		void setBlendMode(SDL_BlendMode blendMode);
		void setAlpha(Uint8 alpha);
		void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
		int getWidth();
		int getHeight();
		bool lockTexture();
		bool unlockTexture();
		void* getPixels();
		int getPitch();

	private:
		SDL_Texture* mTexture;
		int mWidth, mHeight;
		void* mPixels;
		int mPitch;

};