#pragma once
#include <string>

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <Component/SimpleScene.h>

#define TRIANGLE_LENGTH 40

/*
* This class represents a shuriken
* It is formed by combining 4 triangles
*/
class Shuriken
{
public:
	Shuriken(glm::vec3 center, glm::vec3 color);
	~Shuriken();

	Mesh* getTriangle1();
	Mesh* getTriangle2();
	Mesh* getTriangle3();
	Mesh* getTriangle4();

	glm::vec3 getCenter();
	int getTranslateOX();

	int getCircleX();
	int getCircleY();
	int getRadius();
	float getScale();

	void setCenter(glm::vec3 position);
	void setTranslateOX(int x);

	void setCircleX(int x);
	void setCircleY(int y);
	void setRadius(int r);

	void setDestroyed(bool destroyed);
	void setScale(float scale);

	bool isDestroyed();

private:
	Mesh* triangle1;
	Mesh* triangle2;
	Mesh* triangle3;
	Mesh* triangle4;

	// initial position
	glm::vec3 center;
	
	int translateOX;

	// surrounding circle params
	int circleX;
	int circleY;
	int radius;

	bool destroyed;
	float scale;

protected:
	Mesh* CreateTriangle(std::string name, glm::vec3 position, int number, glm::vec3 color);
};
