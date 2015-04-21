#pragma once

#include <GameObject.h>

struct Vertex;

class Projectile : public GameObject
{
public:
  Projectile();
  Projectile(Vector3 position);

  void Initialize(Graphics *graphics);
  void Update(float dt);
  void Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt);
  
  void Move(Direction direction, float dt);
  void SetPosition(Vector3 position);

  void SetSpeed(float speed);
  bool IsOutBounds();
protected:
  //void *_vertexPointer;

  Vector3 *vertices;
  Vector4 *colours;

  int size;
  int offset;
  float speed = 12;

  //Vertex *vertices;
  unsigned int* indices;

  void SetVertex(int index, float x, float y, float z, float r, float g, float b, float a);
};