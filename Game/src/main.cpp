#include <iostream>
#include "Game.h"

using namespace std;

int main(int argc, char** argv)
{
  GameEngine *engine = GameEngine::CreateInstance();
  engine->Initialize();

  while(engine->_running)
  {
    engine->Update();
    engine->Draw();
  }

  engine->Shutdown();

  return 0;
}