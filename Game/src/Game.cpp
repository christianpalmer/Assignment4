
#include "Game.h"
#include <GameObject.h>
#include <SDL.h>
#include <math.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "SoundHelper.h"
#include <InputManager.h>
#include "Cube.h"
#include "Player.h"
#include "Enemy.h"


#include <string.h>
#include <sstream>
#include <Cameras/Camera.h>
#include <Cameras/PerspectiveCamera.h>
#include <Cameras/OrthographicCamera.h>

#include "Parallax/ParallaxLayer.h"
#include "Parallax/ParallaxSystem.h"

// Initializing our static member pointer.
GameEngine* GameEngine::_instance = nullptr;
Player *player = nullptr;
class SoundHelper;

bool messageShowed = false;
float lastSpawn = SPAWN_TIME;


Enemy* enemies[MAX_ENEMIES] = { 0 };

GameEngine* GameEngine::CreateInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Game();
	}
	return _instance;
}

Game::Game() : GameEngine()
{

}

Game::~Game()
{
	delete _parallaxCamera;
	_parallaxCamera = nullptr;

	ParallaxLayer *layerToDelete = nullptr;
	while (_backgroundParallaxSystem->LayerCount() > 0)
	{
		// Delete all of the layers inside of our parallax system.
		layerToDelete = _backgroundParallaxSystem->PopLayer();

		delete layerToDelete;
		layerToDelete = nullptr;
	}

	delete _backgroundParallaxSystem;
	_backgroundParallaxSystem;
}

void Game::InitializeImpl()
{
	_windowTitle = new char[25];

	_soundHelper = new SoundHelper();
	_soundHelper->Initialize();
	_soundHelper->PlayMusic();



	float nearPlane = 0.01f;
	float farPlane = 100.0f;
	Vector4 position(0, 0, 2.5f, 0.0f);
	Vector4 lookAt = Vector4::Normalize(Vector4::Difference(Vector4(0.0f, 0.0f, 0.0f, 0.0f), position));
	Vector4 up(0.0f, 1.0f, 0.0f, 0.0f);

	// _gameCamera = new PerspectiveCamera(100.0f, 1.0f, nearPlane, farPlane, position, lookAt, up);
	_gameCamera = new OrthographicCamera(CAMERA_SIZE * -1, CAMERA_SIZE, CAMERA_SIZE, CAMERA_SIZE * -1, nearPlane, farPlane, position, lookAt, up);


	//ParallaxLayer *layer1 = new ParallaxLayer("water.png", Vector2(0, 2));
	//layer1->GetTransform().position = Vector3(0, 20, 0);
	//_backgroundParallaxSystem->PushLayer(layer1);

	player = new Player();
	_objects.push_back(player);
	UpdateTitle();

	enemies[0] = new Enemy();

	_objects.push_back(enemies[0]);
	for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
	{
		(*itr)->Initialize(_graphicsObject);
	}

//	_backgroundParallaxSystem->Initialize(_graphicsObject);
}

void Game::UpdateImpl(float dt)
{
	//SDL_Event evt;
	//SDL_PollEvent(&evt);
	InputManager::GetInstance()->Update(dt);

	if (InputManager::GetInstance()->GetKeyState(SDLK_UP, SDL_KEYDOWN) == true)
	{
		player->Move(UP, dt);
	}
	else if (InputManager::GetInstance()->GetKeyState(SDLK_DOWN, SDL_KEYDOWN) == true)
	{
		player->Move(DOWN, dt);

	}
	if (InputManager::GetInstance()->GetKeyState(SDLK_LEFT, SDL_KEYDOWN) == true)
	{
		player->Move(LEFT, dt);
	}
	else if (InputManager::GetInstance()->GetKeyState(SDLK_RIGHT, SDL_KEYDOWN) == true)
	{
		player->Move(RIGHT, dt);

	}

	for (int enemyIndex = 0; enemyIndex < MAX_ENEMIES && enemies[enemyIndex] != nullptr; enemyIndex++)
	{

		enemies[enemyIndex]->SetTargetPosition(player->GetTransform().position);
		if (enemies[enemyIndex] != nullptr && enemies[enemyIndex]->IsAlive())
		{
			enemies[enemyIndex]->Shoot();
		}
	}



	if (InputManager::GetInstance()->GetKeyState(SDLK_SPACE, SDL_KEYDOWN) == true)
	{
		_soundHelper->PlaySound(SHOOT);
		player->Shoot();
	}

	for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
	{
		if ((*itr) != nullptr)
		{
			(*itr)->Update(dt);
		}
	}



	if (lastSpawn >= SPAWN_TIME)
	{
		for (int enemyIndex = 0; enemyIndex < MAX_ENEMIES; enemyIndex++)
		{
			if (enemies[enemyIndex] == nullptr || !enemies[enemyIndex]->IsAlive())
			{
				bool exists = enemies[enemyIndex] != nullptr;
				if (exists)
					delete enemies[enemyIndex];

				enemies[enemyIndex] = new Enemy();
				enemies[enemyIndex]->Initialize(_graphicsObject);

				if (!exists)
					_objects.push_back(enemies[enemyIndex]);
				break;
			}
		}
		lastSpawn = 0;
	}

	lastSpawn += dt;

//	_backgroundParallaxSystem->Update(Vector2(0, -1), dt);

}

void Game::DrawImpl(Graphics *graphics, float dt)
{
	std::vector<GameObject *> renderOrder = _objects;
	//CalculateDrawOrder(renderOrder);

	glPushMatrix();

	CalculateCameraViewpoint(_gameCamera);

	for (auto itr = renderOrder.begin(); itr != renderOrder.end() && (*itr) != nullptr; itr++)
	{
		(*itr)->Draw(graphics, _gameCamera->GetProjectionMatrix(), dt);
	}

	glPopMatrix();

	for (int enemyIndex = 0; enemyIndex < MAX_ENEMIES && enemies[enemyIndex] != nullptr; enemyIndex++)
	{

		if (player->CheckCollision(enemies[enemyIndex]->GetTransform().position))
		{
			player->Score();
			enemies[enemyIndex]->RestartPosition();
			UpdateTitle();
		}

		if (enemies[enemyIndex]->CheckCollision(player->GetTransform().position))
		{
			_soundHelper->PlaySound(DEAD);
			player->Die();
			enemies[enemyIndex]->TargetKilled();
			UpdateTitle();
		}


	}

	if (!player->IsAlive() && !messageShowed)
	{
		_soundHelper->PlaySound(GAMEOVER);
		GameOver();
	}
}

void Game::CalculateDrawOrder(std::vector<GameObject *>& drawOrder)
{
	// SUPER HACK GARBAGE ALGO.
	drawOrder.clear();

	auto objectsCopy = _objects;
	auto farthestEntry = objectsCopy.begin();
	while (objectsCopy.size() > 0)
	{
		bool entryFound = true;
		for (auto itr = objectsCopy.begin(); itr != objectsCopy.end(); itr++)
		{
			if (farthestEntry != itr)
			{
				if ((*itr)->GetTransform().position.y < (*farthestEntry)->GetTransform().position.y)
				{
					entryFound = false;
					farthestEntry = itr;
					break;
				}
			}
		}

		if (entryFound)
		{
			GameObject *farthest = *farthestEntry;

			drawOrder.push_back(farthest);
			objectsCopy.erase(farthestEntry);
			farthestEntry = objectsCopy.begin();
		}
	}
}

void Game::CalculateCameraViewpoint(Camera *camera)
{
	camera->Apply();

	Vector4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
	Vector4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);

	Vector3 cameraVector(camera->GetLookAtVector().x, camera->GetLookAtVector().y, camera->GetLookAtVector().z);
	Vector3 lookAtVector(0.0f, 0.0f, -1.0f);

	Vector3 cross = Vector3::Normalize(Vector3::Cross(cameraVector, lookAtVector));
	float dot = MathUtils::ToDegrees(Vector3::Dot(lookAtVector, cameraVector));

	glRotatef(cross.x * dot, 1.0f, 0.0f, 0.0f);
	glRotatef(cross.y * dot, 0.0f, 1.0f, 0.0f);
	glRotatef(cross.z * dot, 0.0f, 0.0f, 1.0f);

	glTranslatef(-camera->GetPosition().x, -camera->GetPosition().y, -camera->GetPosition().z);
}


void Game::UpdateTitle()
{

	//updating the title with the current life and level
	std::string title = "Lego Shooter - Score: " + std::to_string(player->GetScore()) + " | Life:" + std::to_string(player->GetLife());
	strcpy(_windowTitle, title.c_str());
	SDL_SetWindowTitle(_window, _windowTitle);
}


void Game::GameOver()
{

	//displaying a messagebox asking the user if he wants to play again..
	int buttonid;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
		SDL_Log("Error displaying message box");
	}

	messageShowed = true;

	if (buttons[buttonid].buttonid == 0)
	{
		GameEngine::Shutdown();

		return;
	}
	else if (buttons[buttonid].buttonid == 1)
	{
		Restart();
	}
}


void Game::Restart()
{
	//restarting the game
	_objects.clear();

	messageShowed = false;

	player = new Player();
	player->Initialize(_graphicsObject);





	for (int enemyIndex = 0; enemyIndex < MAX_ENEMIES; enemyIndex++)
	{
		delete enemies[enemyIndex];
		enemies[enemyIndex] = nullptr;
	}

	enemies[0] = new Enemy();
	enemies[0]->Initialize(_graphicsObject);

	_objects.push_back(player);
	_objects.push_back(enemies[0]);

	UpdateTitle();

}