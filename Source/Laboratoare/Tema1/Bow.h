#include <string>

#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <Component/SimpleScene.h>

#define ARROWLENGTH 100
#define BOW_ARROW_DIST 60
#define POWER_BAR_LENGTH 40
#define ARROW_TRIANGLE_LENGTH 15

/*
* This class represents a bow and arrow
* The bow is formed with a semicircle and a vertical line
* The arrow is formed with a horizontal line and a triangle
* The powerbar is formed with a scaled rectangle
*/
class Bow
{
public:
	Bow(glm::vec3 corner, glm::vec3 color);
	~Bow();

	glm::vec3 getCorner();

	Mesh* getBowLine();
	Mesh* getBowArc();

	Mesh* getArrowLine();
	Mesh* getTriangle();

	Mesh* getPowerBar();
	
	int getCircleX();
	int getCircleY();
	int getRadius();

	int getArrowX();
	int getArrowY();

	float getSpeedFactor();
	float getPowerBarScale();

	bool isInAir();

	void setCircleX(int x);
	void setCircleY(int y);
	void setRadius(int r);
	void setCorner(glm::vec3 position);
	
	void setArrowX(int x);
	void setArrowY(int y);

	void setSpeed(float speed);
	void setPowerBarScale(float scale);

	void setInAir(bool inAir);

private:
	glm::vec3 corner;

	Mesh* arrowLine;
	Mesh* arrowTriangle;

	Mesh* bowString;
	Mesh* bowArc;
	Mesh* powerBar;

	// surrounding circle params
	int circleX;
	int circleY;
	int radius;

	int arrowX;
	int arrowY;

	// arrow speed
	float speedFactor;

	float powerBarScale;

	// check if arrow was thrown
	bool inAir;

protected:
	Mesh* CreateBowLine(std::string name, glm::vec3 position, bool type, glm::vec3 color);
	Mesh* CreateTriangle(std::string name, glm::vec3 position, glm::vec3 color);
	Mesh* CreateArc(std::string name, glm::vec3 position, glm::vec3 color);
	Mesh* CreatePowerBar(glm::vec3 position, glm::vec3 color);
};
