#pragma once
#include <Component/SimpleScene.h>
#include "Laboratoare/Laborator5/LabCamera.h"
#include "Laboratoare/Tema2/Platform.h"
#include "Player.h"

#define CAMERA_SPEED_UP 5
#define CAMERA_MAX_SPEED 5
#define CAMERA_MIN_SPEED 1

#define Z_LIM 16

#define MAX_JUMP 1.3

#define MAX_PLATFORM_LENGTH 5
#define MIN_PLATFORM_LENGTH 3

#define TILE_GAP 1

#define CONFIGURATIONS_NUM 6

#define MAX_FUEL 100 

#define BUFF_TIME 10

#define BLUE_BUFF 0
#define GREEN_BUFF 1
#define YELLOW_BUFF 2
#define ORANGE_BUFF 3
#define RED_BUFF 4

#define RESPAWN_TIME 3

#define OBSTACLE_DISTANCE 10
#define OBSTACLE_BUFF 5

#define LIFE_BUFF 6

struct configuration {
	int number;
	bool left;
	bool middle;
	bool right;

	std::vector<int> invalid_configurations;
};

struct fuel_bar {
	Mesh* rectangle_back;
	Mesh* rectangle_front;
};


class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;
	glm::mat4 orthoMatrix;



private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
	void RenderMesh2D(Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix) override;


	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color);

	void getConfigurations();
	void get_configuration_number(int& config_num, int& prev_config_num);

	void modify_platform(std::vector<Platform*>& platform_line, int ct, glm::vec3 position_offset, double platform_length, int config_num, glm::vec3 color, int color_name, bool special);
	void Tema2::replace_platform(std::vector<Platform*>& platform_line, std::vector<Platform*>& platform);
	void delete_old_platforms(std::vector<int> to_delete);
	float generatePlatformLength();
	void generateInitialPlatforms();
	void generate_color(glm::vec3& color, int& color_name);

	void apply_buff(float deltaTimeSeconds);
	void translateSphere(float deltaTimeSeconds);
	void renderFuelBar();
	void renderPlatforms();

	void deadAnimation(float deltaTimeSeconds);

	Mesh* createRectangle(glm::vec3 color);
	Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);
	void createLives();


	struct configuration createConfiguration(int num, bool left, bool right, bool midde);


protected:
	Laborator::Camera* camera;
	Laborator::Camera* camera2D;

	glm::mat4 projectionMatrix;
	bool renderCameraTarget;
	int factor = 100;

	float left, right, bottom, top, zNear, zFar, fov;

	bool perspective = true;

	glm::vec3 camera_first_offset = glm::vec3(0, 0.5, 0);

	glm::vec3 camera_offset = glm::vec3(0, 2, 3);

	double times = 0;

	// array with possible platforms configurations
	std::vector<struct configuration> configurations;

	std::vector<std::vector<Platform*>> platforms;

	int lives = 3;

	bool firstPerson = true;

	Mesh* rectangle;
	Mesh* life1;
	Mesh* life2;
	Mesh* life3;

	Mesh* extraLife;

	struct fuel_bar fuelBar;

	Player* player;

	bool fell;

	float time_sec = 0;
};
