#pragma once

#include "Utils.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>

class Paddle
{
public:
	Paddle(int i);
	void Update();
	void SetDir(int d);
	void IncScore();
	void ShowScore(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color);

	Vec2 GetPos();
	int GetId();
	SDL_Rect* GetRect();

private:
	int id; // para ver que pala actualizar
	int dir = 0;
	int width = 20;
	int height = 140;
	float speed;
	int score = 0;

	// Mix_Chunk* sound2 = nullptr;

	Vec2 pos;
	SDL_Rect rect;
	SDL_Rect scoreRect;
	SDL_Surface* scoreSurface = nullptr;
	SDL_Texture* scoreTexture = nullptr;
};

