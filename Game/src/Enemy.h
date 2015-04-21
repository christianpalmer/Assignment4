/**
* \class Enemy.h
* \brief An class that represents a Enemy
* to be placed and manipulated in the world.
* \author Christian Andrade
* \date March 23, 2015
*/

#pragma once

#include <GameObject.h>
#include "Projectile.h"

struct Vertex;

class Enemy : public GameObject
{

private: 

	bool isAlive = true;
	Projectile* _projectile;
	float speed = 1;
	float Xspeed = 1.3;
	bool isShooting = false;
	float lastShot = 0;

	Direction _direction = LEFT;
	Vector3 targetPosition;
public:
	Enemy();
	Enemy(Vector3 position);
	bool CheckCollision(Vector3 pos);

	void SetTargetPosition(Vector3 position);
	void Shoot();
	void TargetKilled();

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
  * \brief A function used to move the Enemy
  * \param direction, the direction to which the Enemy will move
  */
  void Move(float dt);

  /**
  * \fn void Die()
  * \brief A function used when the Enemy die
  */
  void Die();


  /**
  * \fn void RestartPosition()
  * \brief A function used to restart the Enemy position
  */
  void RestartPosition();


  /**
  * \fn void GetLife()
  * \brief A function used get the Enemy life
    \return the number of life
  */
  int GetLife();


  /**
  * \fn void IsAlive()
  * \brief A function used to check if the Enemy is alive
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