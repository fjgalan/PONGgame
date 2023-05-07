#pragma once

#include <SDL_mixer.h>
#include "Paddle.h"
#include "Game.h"

class Ball
{
public:
	Ball(class Game* g);
	void Update(Paddle* lPaddle, Paddle *rPaddle);
	void HandleCollision(Paddle* paddle);
	bool BottomCollision(Paddle* paddle);
	bool TopCollision(Paddle* paddle);
	void Shutdown();
	void Reset();

	SDL_Rect* GetRect();

private:
	int dir = 0;
	int size = 20;
	float speed;
	const int INITIAL_SPEED = 2.f;
	const int MAX_SPEED = 10.f;

	Vec2 pos;
	Vec2 vel;
	SDL_Rect rect;

	Mix_Chunk* sound = nullptr, * yeah = nullptr, *bgm = nullptr, *siuuu = nullptr;

	Game* game;
};