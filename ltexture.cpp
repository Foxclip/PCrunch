#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "ltexture.h"
#include "globals.h"

LTexture::LTexture() {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

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
	//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));
	SDL_Surface* formattedSurface = SDL_ConvertSurface(loadedSurface, SDL_GetWindowSurface(gWindow.getSDLWindow())->format, NULL);
	if(formattedSurface == NULL) {
		printf("Surface conversion error: %s\n", SDL_GetError());
		return false;
	}
	SDL_Texture* newTexture = SDL_CreateTexture(gRenderer, SDL_GetWindowPixelFormat(gWindow.getSDLWindow()),
		SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
	if(newTexture == NULL) {
		printf("Texture creation error: %s SDL error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}
	SDL_LockTexture(newTexture, NULL, &mPixels, &mPitch);
	memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);
	SDL_UnlockTexture(newTexture);
	mPixels = NULL;
	mWidth = formattedSurface->w;
	mHeight = formattedSurface->h;
	SDL_FreeSurface(formattedSurface);
	mTexture = newTexture;
	SDL_FreeSurface(loadedSurface);
	return true;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
	free();
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if(textSurface == NULL) {
		printf("Text loading error: %s\n", TTF_GetError());
		return false;
	}
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
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
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}

bool LTexture::lockTexture() {
	if(mPixels) {
		printf("Texture is already locked\n");
		return false;
	}
	if(SDL_LockTexture(mTexture, NULL, &mPixels, &mPitch) != 0) {
		printf("Texture locking error\n");
		return false;
	}
	return true;
}

bool LTexture::unlockTexture() {
	if(mPixels == NULL) {
		printf("Texture is not locked\n");
		return false;
	}
	SDL_UnlockTexture(mTexture);
	mPixels = NULL;
	mPitch = 0;
	return true;
}

void* LTexture::getPixels() {
	return mPixels;
}

int LTexture::getPitch() {
	return mPitch;
}
