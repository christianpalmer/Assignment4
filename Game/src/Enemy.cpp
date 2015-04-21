#include "Enemy.h"
#include <SDL_opengl.h>
#include <iostream>
#include <InputManager.h>
#include <time.h>

struct Vertex
{
	Vector3 position;
	Vector4 color;
};

Enemy::Enemy()
{
	srand(time(NULL));
	int x = (rand() % 9) + 1;
	_transform.position = Vector3(x, 10, 0);
	_transform.rotation = Vector3::Zero();
	_transform.scale = Vector3(SPACESHIP_SIZE, SPACESHIP_SIZE, SPACESHIP_SIZE);
}


void Enemy::TargetKilled()
{
	isShooting = false;
}




bool Enemy::CheckCollision(Vector3 pos)
{
	SDL_Rect* result = new SDL_Rect();

	Vector3 objPos = _transform.position;

	SDL_Rect objRect = SDL_Rect();
	objRect.x = objPos.x;
	objRect.y = objPos.y;
	objRect.w = 1;
	objRect.h = 1;

	SDL_Rect targetRect = SDL_Rect();
	targetRect.x = pos.x;
	targetRect.y = pos.y;
	targetRect.w = 1;
	targetRect.h = 1;

	bool enemyCollision = SDL_IntersectRect(&objRect, &targetRect, result);

	if (enemyCollision)
		return true;
	else
	{
		objPos = _projectile->GetTransform().position;

		objRect.x = objPos.x;
		objRect.y = objPos.y;
		objRect.w = 1;
		objRect.h = 1;

		return SDL_IntersectRect(&objRect, &targetRect, result);
	}

}
void Enemy::Move(float dt)
{
	float movementY = dt * speed;
	float movementX = dt * Xspeed;


	if (_direction == LEFT)
		_transform.position.x -= movementX;
	else if (_direction == RIGHT)
		_transform.position.x += movementX;

	if (_transform.position.x > CAMERA_SIZE - 1)
		_direction = LEFT;

	else if (_transform.position.x  < (CAMERA_SIZE *-1) +1)
		_direction = RIGHT;

		_transform.position.y -= movementY;

}
Enemy::Enemy(Vector3 position)
{
	_transform.position = position;
	_transform.rotation = Vector3::Zero();
	_transform.scale = Vector3::One();
}

//Vertex *vertices = nullptr;

void Enemy::Initialize(Graphics *graphics)
{

	if (GetCurrentTime() % 2)
		_direction = LEFT;
	else
		_direction = RIGHT;
	
	_projectile = new Projectile(_transform.position);
	_projectile->SetSpeed(9);
	size = 0;

	vertices = new Vector3[8]();
	colours = new Vector4[8]();

	SetVertex(0, /*pos*/-0.3f, 0.3f, 0.3f,  /*color*/ 0.7f, 0.0f, 0.2f, 1.0f);
	SetVertex(1, /*pos*/0.3f, 0.3f, 0.3f,   /*color*/ 0.7f, 0.0f, 0.2f, 1.0f);
	SetVertex(2, /*pos*/-0.3f, -0.3f, 0.3f, /*color*/ 0.7f, 0.0f, 0.2f, 1.0f);
	SetVertex(3, /*pos*/0.3f, -0.3f, 0.3f,  /*color*/ 0.7f, 0.0f, 0.2f, 1.0f);
	SetVertex(4, /*pos*/-0.3f, 0.3f, -0.3f, /*color*/ 0.7f, 0.0f, 0.2f, 1.0f);
	SetVertex(5, /*pos*/0.3f, 0.3f, -0.3f,  /*color*/ 0.7f, 0.0f, 0.2f, 1.0f);
	SetVertex(6, /*pos*/-0.3f, -0.3f, -0.3f,/*color*/ 0.7f, 0.0f, 0.2f, 1.0f);
	SetVertex(7, /*pos*/0.3f, -0.3f, -0.3f, /*color*/ 0.7f, 0.0f, 0.2f, 1.0f);


	indices = new unsigned int[30];

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	// front
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;

	// right
	indices[6] = 1;
	indices[7] = 5;
	indices[8] = 3;

	indices[9] = 5;
	indices[10] = 7;
	indices[11] = 3;

	// back
	indices[12] = 5;
	indices[13] = 4;
	indices[14] = 7;

	indices[15] = 4;
	indices[16] = 6;
	indices[17] = 7;

	// left
	indices[18] = 4;
	indices[19] = 0;
	indices[20] = 6;

	indices[21] = 6;
	indices[22] = 0;
	indices[23] = 2;

	// top
	indices[24] = 4;
	indices[25] = 5;
	indices[26] = 0;

	indices[27] = 5;
	indices[28] = 1;
	indices[29] = 0;

	// bottom
	indices[30] = 2;
	indices[31] = 3;
	indices[32] = 6;

	indices[33] = 3;
	indices[34] = 7;
	indices[35] = 6;


	_projectile->Initialize(graphics);
}

void Enemy::Update(float dt)
{
	if (isShooting)
		_projectile->Move(DOWN, dt);

	if (_projectile->IsOutBounds())
		isShooting = false;

	Move(dt);

	if ((_transform.position.x > CAMERA_SIZE)
		|| (_transform.position.y > CAMERA_SIZE)
		|| (_transform.position.x < CAMERA_SIZE * -1)
		|| (_transform.position.y < CAMERA_SIZE * -1))
	{
		RestartPosition();

	}

	if (_transform.position.y > CAMERA_SIZE* -2)
		Xspeed = 2.6;

	if (_transform.position.y > CAMERA_SIZE *-1 + 5 && _transform.position.y < 0)
		speed = 2;
	else
		speed = 1;

}

void Enemy::Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt)
{
	GLenum error = glGetError();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);

	glPushMatrix();
	glTranslatef(_transform.position.x, _transform.position.y, _transform.position.z);
	glRotatef(_transform.rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(_transform.rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(_transform.rotation.z, 0.0f, 0.0f, 1.0f);

	glScalef(_transform.scale.x, _transform.scale.y, _transform.scale.z);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	//glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertices);

	glColorPointer(4, GL_FLOAT, 0, colours);
	//glColorPointer(4, GL_FLOAT, sizeof(Vertex), vertices + sizeof(Vector3));

	int indexCount = sizeof(indices) / sizeof(unsigned int);
	indexCount = 30;
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);

	if (isShooting)
	{
		_projectile->Draw(graphics, relativeTo, dt);
	}
}

void Enemy::SetVertex(int index, float x, float y, float z, float r, float g, float b, float a)
{

	vertices[index] = Vector3(x, y, z);

	colours[index].x = r;
	colours[index].y = g;
	colours[index].z = b;
	colours[index].w = a;
}


void Enemy::RestartPosition()
{
	 speed = 1;
	 Xspeed = 1.3;

	 srand(time(NULL));
	 int x = (rand() % 9) + 1;
	 _transform.position = Vector3(x,10 , 0);


	if (GetCurrentTime() % 2)
		_direction = LEFT;
	else
		_direction = RIGHT;
}

void Enemy::Shoot()
{
	if (!isShooting)
	{
		_projectile->SetPosition(_transform.position);
		this->isShooting = true;
	}


}
void Enemy::Die()
{
	RestartPosition();
	//play sound
}


bool Enemy::IsAlive()
{
	return isAlive;
}


void Enemy::SetTargetPosition(Vector3 position)
{
	this->targetPosition = position;
}