#include "Ball.h"

Ball::Ball(Game* g)
{
	game = g;
	speed = INITIAL_SPEED;

	pos.x = SCREEN_WIDTH / 2 - size / 2;
	pos.y = SCREEN_HEIGHT / 2 - size / 2;

	vel.x = -1;
	vel.y = 1;

	rect.x = (int)pos.x;
	rect.y = (int)pos.y;
	rect.w = size;
	rect.h = size;

	sound = Mix_LoadWAV("Assets/synth.wav");
	yeah = Mix_LoadWAV("Assets/yeah.wav");
	bgm = Mix_LoadWAV("Assets/1d.wav");
	siuuu = Mix_LoadWAV("Assets/siuuu.mp3");

	Mix_Volume(3, 7);
	Mix_PlayChannel(3, bgm, 0);
}

void Ball::Update(Paddle *lPaddle, Paddle *rPaddle)
{
	pos.x += vel.x * speed;
	pos.y += vel.y * speed;

	if (pos.x < 0)
	{
		rPaddle->IncScore();
		Reset();
	}
	else if (pos.x > SCREEN_WIDTH - size)
	{
		lPaddle->IncScore();
		Reset();
	}

	HandleCollision(lPaddle);
	HandleCollision(rPaddle);

	// TOPE DE PANTALLA PARA QUE NO SE VAYA
	if (pos.y < 0)
	{
		pos.y = 0;
		vel.y *= -1;
	}
	else if (pos.y + size > SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - size;
		vel.y *= -1;
	}

	rect.x = (int)pos.x;
	rect.y = (int)pos.y;
}

bool Ball::BottomCollision(Paddle* paddle)
{
	bool passedBoundry = false;		
	bool collided = false;

	if (paddle->GetId() == 0)
		passedBoundry = pos.x < size;
	else if (paddle->GetId() == 1)
		passedBoundry = pos.x > SCREEN_WIDTH - size; // significa ambos que no chocará con top ni bottom

	if (passedBoundry &&													// Passed outer boundry of paddel
		pos.y <= paddle->GetRect()->y + paddle->GetRect()->h &&				// top of ball is inside bottom of paddle
		pos.y + size > paddle->GetRect()->y + paddle->GetRect()->h &&		// bottom of ball is not inside bottom of paddle
		vel.y < 0)															// ball is moving down
	{
		pos.y = paddle->GetRect()->y +paddle->GetRect()->h + 1;				// give a buffer of 1px so doesn't intersect immeditely
		vel.y *= -1;
		rect.y = pos.y;

		collided = true;
	}

	return collided;
}

bool Ball::TopCollision(Paddle* paddle)
{
	bool passedBoundry = false;
	bool collided = false;

	if (paddle->GetId() == 0)
		passedBoundry = pos.x < size;
	else if (paddle->GetId() == 1)
		passedBoundry = pos.x > SCREEN_WIDTH - size; // significa ambos que no chocará con top ni bottom

	if (passedBoundry &&							// Passed outer boundry of paddel
		pos.y + size >= paddle->GetRect()->y &&		// bottom of ball is inside top of paddle
		pos.y < paddle->GetRect()->y &&				// top of ball is not inside top of paddle
		vel.y > 0)									// ball is moving down
	{
		pos.y = paddle->GetRect()->y - size - 1;	// give a buffer of 1px so doesn't intersect immeditely
		vel.y *= -1;
		rect.y = pos.y;

		collided = true;
	}

	return collided;
}

void Ball::HandleCollision(Paddle* paddle)
{
	if (BottomCollision(paddle) || TopCollision(paddle)) // para no seguir con lo de abajo
	{
		Mix_Volume(2, 40);
		Mix_PlayChannel(2, sound, 0);
		return;
	}

	if (pos.x + size >= paddle->GetPos().x && // Lado derecho de la bola choca con la pala izq
		pos.x <= paddle->GetPos().x + paddle->GetRect()->w && // Lado izq de la bola choca con la pala der
		pos.y + size >= paddle->GetPos().y && // bottom ball intersect top paddle
		pos.y <= paddle->GetPos().y + paddle->GetRect()->h) // Top ball intersect bottom paddle
	{
		vel.x *= -1;

		// Botar la bola diferente dependiendo de si toca paleta
		int middlePaddleY = paddle->GetPos().y + paddle->GetRect()->h / 2;
		int middleBallY = pos.y + size / 2;
		int offsetY = middlePaddleY - middleBallY;

		vel.y -= offsetY * 1.5 / 100.f;

		if (paddle->GetId() == 0) //left paddle
			pos.x = paddle->GetPos().x + paddle->GetRect()->w;
		else if (paddle->GetId() == 1)
			pos.x = paddle->GetPos().x - size;

		Mix_Volume(2, 40);
		Mix_PlayChannel(2, sound, 0);

		if (speed < MAX_SPEED)
			speed++;
	}
	//else
	//{
	//	Mix_Volume(-1, 1);
	//	Mix_PlayChannel(-1, bgm, 0);
	//}
		

}


SDL_Rect* Ball::GetRect()
{
	return &rect;
}

void Ball::Shutdown()
{
	Mix_FreeChunk(sound);
	Mix_FreeChunk(yeah);
	Mix_FreeChunk(bgm);
	Mix_FreeChunk(siuuu);
}

void Ball::Reset()
{
	Mix_Volume(1, 40);
	Mix_PlayChannel(1, siuuu, 0);

	speed = INITIAL_SPEED;

	pos.x = SCREEN_WIDTH / 2 - size / 2;
	pos.y = SCREEN_HEIGHT / 2 - size / 2;

	rect.x = (int)pos.x;
	rect.y = (int)pos.y;

	game->SetGameStatePending();
}
