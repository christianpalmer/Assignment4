/**
* \class Player.h
* \brief An class that represents a player
* to be placed and manipulated in the world.
* \author Christian Andrade
* \date March 23, 2015
*/

#pragma once

#include <GameObject.h>
#include "Projectile.h"

struct Vertex;
class Projectile;

class Player : public GameObject
{

private: 

	int life = 3;

	Projectile* _projectile;
	float speed = 5;
	bool isShooting = false;
	int _score = 0;

public:
	Player();
	Player(Vector3 position);
	bool CheckCollision(Vector3 pos);
	int GetScore();
	void Score();

	void Shoot();
	/**
	* \fn void Initialize()
	* \brief A virtual function that we’ll use as to define how our
	* object is initialized.
	*/
  void Initialize(Graphics *graphics);

  /**
  * \fn void Update(float dt)
  * \brief A virtual function that we’ll use as to define how our
  * object is updated
  * \param dt The time in fractions of a second since the last pass.
  */
  void Update(float dt);

  /**
  * \fn void Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt)
  * \brief A virtual function that we’ll use as to define how our
  * object is drawn to the screen.
  * \param graphics is an object with the properties used to draw the object.
  * \param relativeTo is a matrix object
  * \param dt The time in fractions of a second since the last pass.
  */

  void Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt);

  /**
  * \fn void Move(Direction direction)
  * \brief A function used to move the player
  * \param direction, the direction to which the player will move
  */
  void Move(Direction direction, float dt);

  /**
  * \fn void Die()
  * \brief A function used when the player die
  */
  void Die();


  /**
  * \fn void RestartPosition()
  * \brief A function used to restart the player position
  */
  void RestartPosition();


  /**
  * \fn void GetLife()
  * \brief A function used get the player life
    \return the number of life
  */
  int GetLife();


  /**
  * \fn void IsAlive()
  * \brief A function used to check if the player is alive
  \return bool if alive, false if dead
  */
  bool IsAlive();


protected:
  //void *_vertexPointer;

  Vector3 *vertices;
  Vector4 *colours;

  int size;
  int offset;

  //Vertex *vertices;
  unsigned int* indices;

  void SetVertex(int index, float x, float y, float z, float r, float g, float b, float a);

};