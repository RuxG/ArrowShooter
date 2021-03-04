#include "..\Source\Laboratoare\Tema2\Platform.h"
#include <Core/Engine.h>

Platform::Platform(glm::vec3 position_offset, double length, int configuration, glm::vec3 color, bool special, int color_name, float obstacleDistance)
{
	this->platform = new Mesh("cube");
	this->platform->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	this->position_offset = position_offset;
	this->length = length;
	this->configuration = configuration;
	this->color = color;
	this->special = special;
	this->color_name = color_name;
	this->obstacle_distance = obstacleDistance;

	if (obstacleDistance > 0) {
		obstacle = new Mesh("cube");
		obstacle->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	}
}

Platform::~Platform()
{
}

Mesh* Platform::getPlatform()
{
	return platform;
}

Mesh* Platform::getObstacle()
{
	return obstacle;
}

glm::vec3 Platform::getPos()
{
	return position_offset;
}

glm::vec3 Platform::getColor()
{
	return color;
}

double Platform::getLength()
{
	return length;
}



bool Platform::getUsedPower()
{
	return usedPower;
}

bool Platform::getSpecial()
{
	return special;
}

int Platform::getColorName()
{
	return color_name;
}


void Platform::setPosition(glm::vec3 position_offset)
{
	this->position_offset = position_offset;
}

void Platform::setLength(double length)
{
	this->length = length;
}

void Platform::setConfiguration(int configuration)
{
	this->configuration = configuration;
}

void Platform::setSpecial(bool special)
{
	this->special = special;
}

void Platform::setObstacle()
{
	obstacle = new Mesh("cube");
	obstacle->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
}

void Platform::setObstacleDistance(float distance)
{
	this->obstacle_distance = obstacle_distance;
}

int Platform::getConfiguration()
{
	return configuration;
}

float Platform::getObstacleDistance()
{
	return obstacle_distance;
}

bool Platform::getHitObstacle()
{
	return hit_obstacle;
}

void Platform::setColor(glm::vec3 color)
{
	this->color = color;
}

void Platform::setUsedPower(bool power)
{
	this->usedPower = power;
}

void Platform::setColorName(int color_name)
{
	this->color_name = color_name;
}

void Platform::setHitObstacle(bool obstacle)
{
	this->hit_obstacle = obstacle;
}

