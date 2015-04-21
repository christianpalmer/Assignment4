#include "SoundHelper.h"

 void SoundHelper::Initialize()
{
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	destroySound = Mix_LoadWAV("sounds/die.wav");
	shootSound = Mix_LoadWAV("sounds/shoot.wav");
	deadSound  = Mix_LoadWAV("sounds/die.wav");
	gameOverSound = Mix_LoadWAV("sounds/gameOver.wav");
	musicSound = Mix_LoadWAV("sounds/music.wav");
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

}


 void SoundHelper::PlayMusic()
 {
 
	 Mix_PlayChannel(1, musicSound, -1);
 }
void SoundHelper::PlaySound(GameSound sound)
{

	switch (sound)
	{
	case SHOOT:
		Mix_PlayChannel(-1, shootSound, 0);
		
	case DEAD:
		Mix_PlayChannel(-1,deadSound, 0);
		break;
	case GAMEOVER:
		Mix_PlayChannel(-1, gameOverSound, 0);
		break;
	case DESTROY:
		Mix_PlayChannel(-1, destroySound, 0);
		break;
	default:
		break;
	}
	
}