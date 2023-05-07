#include "Game.h"
#include "Utils.h"

bool Game::Init()
{
	// INICIALIZACIÓN DE SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		return false;

	// VENTANA CREADA
	window = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!window)
		return false;

	// RENDERER CREADO
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
		return false;

	if (TTF_Init() != 0)
		return false;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512);

	// INICIO DE PALAS
	leftPaddle = new Paddle(0);
	rightPaddle = new Paddle(1);

	// INICIO DE BOLA
	ball = new Ball(this);

	font = TTF_OpenFont("Assets/BrunoAceSC-Regular.ttf", 32);

	return true;
}

void Game::SetGameStatePending()
{
	gameState = gameStates::Pending;
}

void Game::GameLoop()
{
	while (isRunning)
	{
		HandleEvents();
		Update();
		Draw();
	}
}

void Game::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			isRunning = false;
	}

	const Uint8* keyStates = SDL_GetKeyboardState(NULL);

	if (keyStates[SDL_SCANCODE_ESCAPE])
		isRunning = false;


	if (gameState == gameStates::Pending)
	{
		if (keyStates[SDL_SCANCODE_SPACE])
			gameState = gameStates::Playing;
		return;
	}

	// PALA IZQUIERDA MOV
	leftPaddle->SetDir(0); // PARA QUE SE MUEVA SOLO MIENTRAS PRESIONAS TECLA
	if (keyStates[SDL_SCANCODE_W])
		leftPaddle->SetDir(-1);
	if (keyStates[SDL_SCANCODE_S])
		leftPaddle->SetDir(1);

	// PALA DERECHA MOV
	rightPaddle->SetDir(0); // PARA QUE SE MUEVA SOLO MIENTRAS PRESIONAS TECLA
	if (keyStates[SDL_SCANCODE_UP])
		rightPaddle->SetDir(-1);
	if (keyStates[SDL_SCANCODE_DOWN])
		rightPaddle->SetDir(1);
}

void Game::Update()
{
	if (gameState == gameStates::Pending)
		return;

	leftPaddle->Update();
	rightPaddle->Update();

	ball->Update(leftPaddle, rightPaddle);

}

void Game::Draw()
{
	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 225);
	SDL_RenderClear(renderer);

	// DIBUJO DE PALAS
	SDL_SetRenderDrawColor(renderer, 225, 225, 225, 225); // COLOR PALAS
	SDL_RenderFillRect(renderer, leftPaddle->GetRect());
	SDL_RenderFillRect(renderer, rightPaddle->GetRect());

	// DIBUJO BOLA
	SDL_SetRenderDrawColor(renderer, 40, 40, 225, 225);
	SDL_RenderFillRect(renderer, ball->GetRect());

	leftPaddle->ShowScore(renderer, font, fontColor);
	rightPaddle->ShowScore(renderer, font, fontColor);


	SDL_RenderPresent(renderer);
}

void Game::Shutdown()
{
	ball->Shutdown();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}
