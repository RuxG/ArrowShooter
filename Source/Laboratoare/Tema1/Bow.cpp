#include "Bow.h"
#include <gl\GL.h>
#include <iostream>

/*
* @param "corner" is the initial position of the bow
*/
Bow::Bow(glm::vec3 corner, glm::vec3 color)
{
	this->arrowLine = CreateBowLine(std::string("bowLine"), corner, false, color);
	this->bowString = CreateBowLine(std::string("arrowLine"), corner, true, color);
	this->arrowTriangle = CreateTriangle(std::string("triangle"), corner, color);
	this->bowArc = CreateArc(std::string("arc"), corner, color);
	this->powerBar = CreatePowerBar(corner + glm::vec3((-1) * POWER_BAR_LENGTH, (-1) * ARROWLENGTH - 2 * POWER_BAR_LENGTH, 0), color);

	this->corner = corner;
	this->circleX = corner.x + BOW_ARROW_DIST;
	this->circleY = corner.y;
	this->radius = ARROWLENGTH * 2 / 2.5;

	this->powerBarScale = 1;
	this->speedFactor = 0;

	this->arrowX = 0;
	this->arrowY = 0;
}

Bow::~Bow()
{

}

Mesh* Bow::CreateBowLine(std::string name, glm::vec3 position, bool type, glm::vec3 color)
{
	Mesh* line = new Mesh("line");
	
	std::vector<VertexFormat> vertices;
	if (type) {
		vertices = {
		VertexFormat(position - glm::vec3(0, ARROWLENGTH, 0), glm::vec3(1, 1, 0)),
		VertexFormat(position + glm::vec3( 0, ARROWLENGTH, 0), glm::vec3(1, 1, 0)),
		};
	}
	else {
		vertices = {
		VertexFormat(position + glm::vec3(BOW_ARROW_DIST, 0, 0) , glm::vec3(1, 1, 0)),
		VertexFormat(position + glm::vec3(ARROWLENGTH + BOW_ARROW_DIST, 0, 0), glm::vec3(1, 1, 0)),
		};
	}
	
	line->SetDrawMode(GL_LINES);

	std::vector<unsigned short> indices = {
		0, 1
	};

	line->InitFromData(vertices, indices);

	return line; 

}

Mesh* Bow::CreateTriangle(std::string name, glm::vec3 position, glm::vec3 color) {

	Mesh* triangle = new Mesh("triangle");
	triangle->SetDrawMode(GL_TRIANGLES);

	std::vector<VertexFormat> vertices = {
		VertexFormat(position + glm::vec3(ARROWLENGTH + BOW_ARROW_DIST, ARROW_TRIANGLE_LENGTH, 0), glm::vec3(1, 0, 0)),
		VertexFormat(position + glm::vec3(ARROWLENGTH + BOW_ARROW_DIST + ARROW_TRIANGLE_LENGTH,0, 0), glm::vec3(1, 0, 0)),
		VertexFormat(position + glm::vec3(ARROWLENGTH + BOW_ARROW_DIST, (-1) * ARROW_TRIANGLE_LENGTH, 0), glm::vec3(1, 0, 0)),

	};

	std::vector<unsigned short> indices = {
		0, 1, 2
	};

	triangle->InitFromData(vertices, indices);

	return triangle;
}


Mesh* Bow::CreateArc(std::string name, glm::vec3 position, glm::vec3 color)
{
	Mesh* circle = new Mesh("circle");
	circle->SetDrawMode(GL_LINE_STRIP);

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	double angle;
	for (int i = 0; i <= 50; i++) {
		angle = 180 * (float) i / (float) 50 - 90;
		
		angle = 2 * 3.14 * (angle / 360);

		float y = ARROWLENGTH * sin(angle);
		float x = ARROWLENGTH * cos(angle);

		vertices.push_back(VertexFormat(position + glm::vec3(x, y, 0), glm::vec3(1, 1, 0)));
		indices.push_back(i);
	}

	circle->InitFromData(vertices, indices);
	return circle;
}

Mesh* Bow::CreatePowerBar(glm::vec3 corner, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner,  glm::vec3(0, 1, 0)),
		VertexFormat(corner + glm::vec3(POWER_BAR_LENGTH, 0, 0), glm :: vec3(1, 0, 0)),
		VertexFormat(corner + glm::vec3(POWER_BAR_LENGTH , POWER_BAR_LENGTH , 0),  glm::vec3(1, 0, 0)),
		VertexFormat(corner + glm::vec3(0, POWER_BAR_LENGTH , 0), glm::vec3(0, 1, 0))
	};

	Mesh* square = new Mesh("powerBar");
	square->SetDrawMode(GL_TRIANGLE_FAN);
	std::vector<unsigned short> indices = {0, 1, 2, 3};

	square->InitFromData(vertices, indices);
	return square;
}


glm::vec3 Bow::getCorner()
{
	return Bow::corner;
}

Mesh* Bow::getArrowLine()
{
	return Bow::arrowLine;
}


Mesh* Bow::getBowLine()
{
	return Bow::bowString;
}

Mesh* Bow::getBowArc()
{
	return bowArc;
}

Mesh* Bow::getTriangle()
{
	return Bow::arrowTriangle;
}

Mesh* Bow::getPowerBar()
{
	return powerBar;
}

int Bow::getCircleX()
{
	return circleX;
}

int Bow::getCircleY()
{
	return circleY;
}

int Bow::getRadius()
{
	return radius;
}

int Bow::getArrowX()
{
	return arrowX;
}

int Bow::getArrowY()
{
	return arrowY;
}

bool Bow::isInAir()
{
	return inAir;
}

float Bow::getSpeedFactor()
{
	return speedFactor;
}

float Bow::getPowerBarScale()
{
	return powerBarScale;
}



void Bow::setCorner(glm::vec3 position)
{
	this->corner = position;
}

void Bow::setArrowX(int x)
{
	this->arrowX = x;
}

void Bow::setArrowY(int y)
{
	this->arrowY = y;
}

void Bow::setCircleX(int x)
{
	circleX = x;
}

void Bow::setCircleY(int y)
{
	circleY = y;
}

void Bow::setRadius(int r)
{
	radius = r;
}

void Bow::setSpeed(float speed)
{
	speedFactor = speed;
}

void Bow::setPowerBarScale(float scale)
{
	powerBarScale = scale;
}

void Bow::setInAir(bool inAir)
{
	this->inAir = inAir;
}
