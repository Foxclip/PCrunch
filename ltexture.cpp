#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "ltexture.h"

LTexture::LTexture() {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::LTexture(SDL_Renderer** renderer) : LTexture() {
	mRenderer = renderer;
}

#ifdef _SDL_TTF_H
LTexture::LTexture(SDL_Renderer** renderer, TTF_Font** font): LTexture() {
	mRenderer = renderer;
	mFont = font;
}
#endif

LTexture::~LTexture() {
	free();
}

bool LTexture::loadFromFile(std::string path) {
	free();
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL) {
		printf("Image (%s) loading error: %s\n", path.c_str(), IMG_GetError());	
		return false;
	}
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(*mRenderer, loadedSurface);
	if(newTexture == NULL) {
		printf("Texture creation error: %s SDL error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
	mTexture = newTexture;
	SDL_FreeSurface(loadedSurface);
	return true;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
	free();
	SDL_Surface* textSurface = TTF_RenderText_Solid(*mFont, textureText.c_str(), textColor);
	if(textSurface == NULL) {
		printf("Text loading error: %s\n", TTF_GetError());
		return false;
	}
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(*mRenderer, textSurface);
	if(newTexture == NULL) {
		printf("Text texture creation error: %s\n", SDL_GetError());
		return false;
	}
	mWidth = textSurface->w;
	mHeight = textSurface->h;
	mTexture = newTexture;
	SDL_FreeSurface(textSurface);
	return true;
}
#endif //_SDL_TTF_H

void LTexture::free() {
	if(mTexture) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blendMode) {
	SDL_SetTextureBlendMode(mTexture, blendMode);
}

void LTexture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if(clip) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(*mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}
