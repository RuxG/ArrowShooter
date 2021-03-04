#pragma once
#include <string>

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <Component/SimpleScene.h>

#define CIRCLE_RANGE 50
#define POLY_DIST 10

/*
	This class represents a balloon
	It's oval shape is built by drawing the intersection of 2 separate circles
*/
class Balloon
{
public:
	/*
	* @param "center" - center of the oval
	* @param vcenter1" - center of the left circle
	* @param "center2" - center of the right circle
	* @param "positiveScore" - whether a balloon gives a positive or negative score
	*/
	Balloon::Balloon(glm::vec3 center, glm::vec3 center1, glm::vec3 center2, glm::vec3 color, bool positiveScore);
	Balloon::~Balloon();

	Mesh* getLeftHalf();
	Mesh* getRightHalf();
	Mesh* getLine();

	glm::vec3 getCenter();
	glm::vec3 getCenter1();
	glm::vec3 getCenter2();

	float getY();
	float getScale();
	int getRadius();

	bool isDestroyed();
	bool isPositiveScore();

	void Balloon::setCenter(glm::vec3 position);
	void Balloon::setCenter1(glm::vec3 position);
	void Balloon::setCenter2(glm::vec3 position);

	void Balloon::setY(float y);

	void setDestroyed(bool destroyed);

	void setScale(float scale);

	void setPositiveScore(bool score);

private:
	// balloon circles coordinates
	glm::vec3 center1;
	glm::vec3 center2;
	glm::vec3 center;
	
	float translateY;
    
	// balloon components meshes
	Mesh* leftHalf;
	Mesh* rightHalf;
	Mesh* polyLine;

	// circle radius and scale
	int radius;
	float scale;

	bool destroyed;
	bool positiveScore;

protected:
	/*
	* Create half of the oval shape.
	* @param "left" == true => draw the left side, else draw the right side
	*/
	Mesh* CreateHalf(bool left, glm::vec3 color);

	// Create the polyline
	Mesh* CreateLine(glm::vec3 color);
};

