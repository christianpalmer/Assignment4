#pragma once 

#include <SDL_mixer.h>

enum GameSound { SHOOT = 0, DEAD, DESTROY, GAMEOVER };

class SoundHelper
{
protected:
	Mix_Chunk* deadSound = nullptr;
	Mix_Chunk* destroySound = nullptr;
	Mix_Chunk* musicSound = nullptr;
	Mix_Chunk* gameOverSound = nullptr;
	Mix_Chunk* shootSound = nullptr;
	

public:

	void Initialize();
	void PlaySound(GameSound sound);
	void PlayMusic();

};