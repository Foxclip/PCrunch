#pragma once

class LTexture {
	public:
		LTexture();
		LTexture(SDL_Renderer** renderer, TTF_Font** font = NULL);
		~LTexture();
		bool loadFromFile(std::string path);
		bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
		void free();
		void setColor(Uint8 red, Uint8 green, Uint8 blue);
		void setBlendMode(SDL_BlendMode blendMode);
		void setAlpha(Uint8 alpha);
		void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
		int getWidth();
		int getHeight();

	private:
		SDL_Renderer** mRenderer;
		TTF_Font** mFont;
		SDL_Texture* mTexture;
		int mWidth, mHeight;
};