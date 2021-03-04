#include "Balloon.h"
#include <iostream>


Balloon::Balloon(glm::vec3 center, glm::vec3 center1, glm::vec3 center2, glm::vec3 color, bool positiveScore)
{
	this->center = center;
	this->center1 = center1;
	this->center2 = center2;

	leftHalf = CreateHalf(true, color);
	rightHalf = CreateHalf(false, color);
	polyLine = CreateLine(color);

	translateY = center.y;

	this->scale = 1;
	this->radius = CIRCLE_RANGE;
	
	this->positiveScore = positiveScore;
}

Balloon::~Balloon()
{
	
}


Mesh* Balloon::CreateHalf(bool left, glm::vec3 color)
{
	
	Mesh* circle = new Mesh("circle");
	circle->SetDrawMode(GL_TRIANGLE_FAN);

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	// compute the intersection
	double angle;

	double dist = (double) (center2.x - center1.x) / 2;
	double u = asin(dist / CIRCLE_RANGE);
	double u_grades = abs(u * 180 / 3.14);	

	for (int i = 0; i <= 50; i++) {

		if (left) {
			double value = 180 * (float)i / (float)50 - 90;
			
			if (value <= -90 + u_grades ) {
				angle = -90 + u_grades;
			} else if (value >= 90 - u_grades) {
				angle = 90 - u_grades;
			}
			else {
				angle = value;
			}
			
		}
		else {
			double value = 180 * (float)i / (float)50 + 90;
			if (value <= 90 + u_grades) {
				angle = 90 + u_grades;
			}
			else if(value >= 270 - u_grades){
				angle = 270 - u_grades;
			}
			else {
				angle = value;
			}
			
		}
		
		angle = 2 * 3.14 * (angle / 360);

		float y = CIRCLE_RANGE * sin(angle);
		float x = CIRCLE_RANGE * cos(angle);

		if (left) {
			vertices.push_back(VertexFormat(center1 + glm::vec3(x, y, 0), color));
		}
		else {
			vertices.push_back(VertexFormat(center2 + glm::vec3(x, y, 0), color));
		}
		indices.push_back(i);
	}

	circle->InitFromData(vertices, indices);
	return circle;
}


Mesh* Balloon::CreateLine(glm::vec3 color)
{
	Mesh* line = new Mesh("line");

	std::vector<VertexFormat> vertices;

	vertices = {
		VertexFormat(center - glm::vec3(0, CIRCLE_RANGE, 0) , glm::vec3(1, 1, 0)),
		VertexFormat(center - glm::vec3(POLY_DIST ,  CIRCLE_RANGE + POLY_DIST, 0), glm::vec3(1, 1, 0)),
		VertexFormat(center - glm::vec3((-1) * POLY_DIST, CIRCLE_RANGE + POLY_DIST * 2, 0), glm::vec3(1, 1, 0)),
		VertexFormat(center - glm::vec3(POLY_DIST, CIRCLE_RANGE + POLY_DIST * 3, 0), glm::vec3(1, 1, 0)),
		VertexFormat(center - glm::vec3((-1) * POLY_DIST, CIRCLE_RANGE + POLY_DIST * 4, 0), glm::vec3(1, 1, 0))
	};

	line->SetDrawMode(GL_LINE_STRIP);

	std::vector<unsigned short> indices = {
		0, 1, 2, 3, 4
	};

	line->InitFromData(vertices, indices);

	return line;
}

Mesh* Balloon::getLeftHalf()
{
	return leftHalf;
}

Mesh* Balloon::getRightHalf()
{
	return rightHalf;
}

Mesh* Balloon::getLine()
{
	return polyLine;
}


glm::vec3 Balloon::getCenter()
{
	return center;
}

glm::vec3 Balloon::getCenter1()
{
	return center1;
}

glm::vec3 Balloon::getCenter2()
{
	return center2;
}

int Balloon::getRadius()
{
	return radius;
}

bool Balloon::isDestroyed()
{
	return destroyed;
}

bool Balloon::isPositiveScore()
{
	return positiveScore;
}

float Balloon::getY()
{
	return translateY;
}


float Balloon::getScale()
{
	return scale;
}

void Balloon::setCenter(glm::vec3 position)
{
	center = position;
}

void Balloon::setCenter1(glm::vec3 position)
{
	center1 = position;
}

void Balloon::setCenter2(glm::vec3 position)
{
	center2 = position;
}

void Balloon::setY(float y)
{
	this->translateY = y;
}


void Balloon::setDestroyed(bool destroyed)
{
	this->destroyed = destroyed;
}

void Balloon::setScale(float scale)
{
	this->scale = scale;
}

void Balloon::setPositiveScore(bool score)
{
	this->positiveScore = score;
}
