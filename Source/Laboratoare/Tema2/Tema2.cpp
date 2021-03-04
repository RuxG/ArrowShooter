#include "Tema2.h"
#include "Laboratoare/Laborator4/Transform3D.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <ctime>
#include "Laboratoare/Laborator3/Object2D.h"
#include "Laboratoare/Laborator3/Laborator3_Vis2D.h"
#include "Laboratoare/Laborator3/Transform2D.h"
#include <vector>


#include <Core/Engine.h>


using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}


int coinToss(float prob) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::bernoulli_distribution d(prob);
	int res = d(gen);

	return res;
}



int randomNum(int max) {
	int randomNumber = 0;
	
	if (max == 0) {
		max = 1;
	}
	randomNumber += ((rand() + rand() + rand()) % max) + 1;

		
	return randomNumber;
}



void Tema2::Init()
{
	renderCameraTarget = false;

	camera = new Laborator::Camera();
	camera->Set(glm::vec3 (0, 0.25, 2) + camera_first_offset, glm::vec3(0, 1, 0) , glm::vec3(0, 1, 0));

	glm::ivec2 resolution = window->GetResolution();
	camera2D = new Laborator::Camera();
	camera2D->Set(glm::vec3(0, 0, 50.f), glm::vec3(0, 0, 25.f), glm::vec3(0, 1.f, 0));
	

	// 2D and 3D camera projection matrixes
	orthoMatrix = glm::ortho(0.f, (float)resolution.x, 0.f, (float)resolution.y, 0.01f, 400.f);

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	// player
	glm::vec3 position(0, 0.25, 2);
	player = new Player(position, CAMERA_MIN_SPEED, MAX_FUEL);
	

	// create tiles configurations
	getConfigurations();

	// generate platforms
	generateInitialPlatforms();

	// generate fuel bar
	fuelBar.rectangle_back = createRectangle(glm::vec3 (1, 1, 1));
	fuelBar.rectangle_front = createRectangle(glm::vec3(0, 1, 0));

	left = -10;
	right = 10;
	bottom = -10;
	top = 10;
	zNear = 0.01f;
	zFar = 200.0f;
	fov = RADIANS(60);

	//Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;

		Shader* shader2 = new Shader("ShaderTema2Deformare");
		shader2->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader-Sphere.glsl", GL_VERTEX_SHADER);
		shader2->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader-Sphere.glsl", GL_FRAGMENT_SHADER);
		shader2->CreateAndLink();
		shaders[shader2->GetName()] = shader2;
	}
	
	createLives();

}


void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds) {	

	if (player->isDead()) {
		deadAnimation(deltaTimeSeconds);
	} 

	apply_buff(deltaTimeSeconds);

	translateSphere(deltaTimeSeconds); 

	renderPlatforms();

	renderFuelBar();
	
}


void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::OnInputUpdate(float deltaTime, int mods) {

		if (window->KeyHold(GLFW_KEY_W)) {
			// increase speed
			player->setSpeed(player->getSpeed() + deltaTime * CAMERA_SPEED_UP);
			if (player->getSpeed() > CAMERA_MAX_SPEED) {
				player->setSpeed(CAMERA_MAX_SPEED);
			}

		}

		if (window->KeyHold(GLFW_KEY_A) && !player->isDead()) {
			// modify player offset to the left
			camera->TranslateRight((-1) * deltaTime * player->getSpeed());
			player->setSpherePosition(player->getPosition() - glm::vec3(deltaTime * player->getSpeed(), 0, 0));
			player->setOffset(player->getOffset() - glm::vec3(deltaTime * player->getSpeed(), 0, 0));
			player->setSphereRotation(player->getRotationAngle()  - glm::vec3(0, 0, deltaTime * player->getSpeed()));
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// decrease speed
			player->setSpeed(player->getSpeed() - deltaTime * CAMERA_SPEED_UP);
			if (player->getSpeed() < CAMERA_MIN_SPEED) {
				player->setSpeed(CAMERA_MIN_SPEED);
			}
		}

		if (window->KeyHold(GLFW_KEY_D) && !player->isDead()) {
			// modify player offset to the right
			camera->TranslateRight(deltaTime * player->getSpeed());
			player->setSpherePosition(player->getPosition() + glm::vec3(deltaTime * player->getSpeed(), 0, 0));
			player->setOffset(player->getOffset() + glm::vec3(deltaTime * player->getSpeed(), 0, 0));
			player->setSphereRotation(player->getRotationAngle() +  glm::vec3(0, 0, deltaTime * player->getSpeed()));

		}

		if (window->KeyHold(GLFW_KEY_SPACE) && !player->isDead()) {
			double offset = player->getPosition().y + deltaTime * player->getSpeed();
			player->setInAir(true);

			// if max height was reached, start descending
			if (offset >= MAX_JUMP) player->setDescend(true);

			// move upword
			if (!player->isDescending()) {
				camera->TranslateUpword(deltaTime * player->getSpeed());
				player->setSpherePosition(player->getPosition() + glm::vec3(0, deltaTime * player->getSpeed(), 0));
			}
			
		}
}

void Tema2::OnKeyPress(int key, int mods)
{	
	// change camera perspective
	if (key == GLFW_KEY_Q) {
		renderCameraTarget = !renderCameraTarget;
		if (renderCameraTarget) {
			camera->Set(player->getPosition()  + camera_offset, glm::vec3(0, 1, 0) + player->getOffset(), glm::vec3(0, 1, 0));
		}
		else {
			camera->Set(player->getPosition()  + camera_first_offset, glm::vec3(0, 1, 0) + player->getOffset(), glm::vec3(0, 1, 0));
		}
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	if (key == GLFW_KEY_SPACE) {
		player->setDescend(true);
		player->setInAir(true);
	}
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}


double min(double a, double b) {
	return a < b ? a : b;
}

double max(double a, double b) {
	return a > b ? a : b;
}

void Tema2::renderPlatforms() {
	int size = platforms.size();

	// platforms that will be deleted after current frame
	std::vector<int> to_delete;

	// variable used for detecting the collision between player and platforms
	bool collision = false;
	for (int i = 0; i < size; i++) {
		std::vector<Platform*> platform_line = platforms[i];

		// variable used for detecting if a platform is behind the player
		// and should be deleted
		bool rendered = false;
		for (int j = 0; j < platform_line.size(); j++) {

			// platform z offset from start 
			glm::vec3 offset = platform_line[j]->getPos();

			// platform length
			double length = platform_line[j]->getLength();

			// distance from player
			double distance = player->getOffset().z + (-1) * Z_LIM;

			// platform is not in sight, skip it
			if ((-1) * offset.z < distance) {
				break;
			}

			// platform is behind the player, mark it as "rendered" and skip
			else if (player->getPosition().z < -offset.z - 5) {
				rendered = true;
				to_delete.push_back(i);
				break;
			}

			// platform is in sight 
			else {
				// save platform color
				glm::vec3 original_color = platform_line[j]->getColor();

				// check if player is within platform's width and length
				if ((player->getPosition().z <= -offset.z + length / 2 + 0.125) && (player->getPosition().z >= -offset.z - length / 2 - 0.125) &&
					player->getPosition().x <= offset.x + 0.65 && player->getPosition().x >= offset.x - 0.65) {

					// check if player is not in mid-air
					if (!player->isInAir()) {
						
						// check if the platform has special powers that were not already used
						if (platform_line[j]->getSpecial() && !platform_line[j]->getUsedPower()) {
							// orange tile -> set speed
							if (player->getBuffType() == ORANGE_BUFF) {
								player->setSpeed(2.5);
							}

							// mark power as used
							platform_line[j]->setUsedPower(true);

							// set special buff timer
							player->setBuffType(platform_line[j]->getColorName());
							player->setBuffTime(BUFF_TIME);

							// life tile -> + 1 lives
							if (player->getBuffType() == LIFE_BUFF) {
								lives = min(lives + 1, 3);
								player->setBuffTime(0);
								player->setBuffType(BLUE_BUFF);
							}
							
						}
						// make platform purple
						platform_line[j]->setColor(glm::vec3(0.7, 0, 1));
					}

					// the player is on the platform
					collision = true;
				}

				// render the platform
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(offset.x, offset.y, -offset.z);
				modelMatrix *= Transform3D::Scale(1, 0.2, platform_line[j]->getLength());
				RenderSimpleMesh(platform_line[j]->getPlatform(), shaders["ShaderTema2"], modelMatrix, platform_line[j]->getColor());

				// check if the platform has an obstacle and if the player hit it
				// render the obstacle
				if (platform_line[j]->getObstacleDistance() > 0 && !platform_line[j]->getHitObstacle()) {
					// the player hit the obstacle
					if (player->getPosition().z <= -offset.z + 0.1 && player->getPosition().z >= -offset.z - 0.1
						&& player->getPosition().y <= offset.y + 1 &&
						player->getPosition().x <= offset.x + 0.5 && player->getPosition().x >= offset.x - 0.5) {

						// decrease lives
						lives--;

						// mark obstacle as hit
						platform_line[j]->setHitObstacle(true);
						platform_line[j]->setObstacleDistance(0);

					}
					// render the obstacle
					modelMatrix = glm::mat4(1);
					modelMatrix *= Transform3D::Translate(offset.x, offset.y + 0.5, -offset.z);
					modelMatrix *= Transform3D::Scale(1, 1, 0.2);
					RenderSimpleMesh(platform_line[j]->getObstacle(), shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 1, 1));
				}

				// change back the platform's color
				platform_line[j]->setColor(original_color);
			}
		}

		// generate a new line of platforms if it is behind the player
		if (rendered) {
			std::vector<Platform*> platform;
			replace_platform(platform_line, platform);
			platforms.push_back(platform);
		}
	}

	// delete old platforms
	delete_old_platforms(to_delete);

	// mark player as dead if he fell
	if (!collision && !player->isInAir() && !player->isDead()) {
		player->setDead(true);
	}
}

void Tema2::generateInitialPlatforms() {
	int prev_config_num = -1;
	int config_num = -1;

	double gap = 0;
	double max_offset = 0;

	// create initial platforms
	for (int i = 0; i < 8; i++) {
		std::vector<Platform*> platform;

		// generate a platform line configuration number
		prev_config_num = config_num;
		get_configuration_number(config_num, prev_config_num);

		if (i == 0) config_num = 0;

		// generate platform length
		double platform_length = generatePlatformLength();

		if (i == 0) platform_length = MAX_PLATFORM_LENGTH * 0.7;

		// generate platform gap
		gap = coinToss(0.5);
		if (i == 0) gap = 0;
		max_offset += gap;

		if (i != 0) max_offset += platform_length / 2.0;

		// generate platform color
		glm::vec3 color = glm::vec3(0, 0, 1);
		int color_name = BLUE_BUFF;
		generate_color(color, color_name);

		// left tile
		if (configurations[config_num].left) {
			platform.push_back(new Platform(glm::vec3((-1.7) * TILE_GAP, -0.5, max_offset), platform_length, config_num, glm::vec3(0, 0, 1), false, BLUE_BUFF, 0));
		}
		else {
			// randomize if another tile will be spawn
			int additional_tile = coinToss(0.5);
			if (additional_tile) {
				platform.push_back(new Platform(glm::vec3((-1.7) * TILE_GAP, -0.5, max_offset), platform_length, config_num, color, true, color_name, 0));
			}
		}

		// middle tile
		if (configurations[config_num].middle) {
			platform.push_back(new Platform(glm::vec3(0, -0.5, max_offset), platform_length, config_num, glm::vec3(0, 0, 1), false, BLUE_BUFF, 0));
		}
		else {
			// randomize if another tile will be spawn
			int additional_tile = coinToss(0.5);
			if (additional_tile) {
				platform.push_back(new Platform(glm::vec3(0, -0.5, max_offset), platform_length, config_num, color, true, color_name, 0));
			}
		}

		// right tile
		if (configurations[config_num].right) {
			platform.push_back(new Platform(glm::vec3(1.7 * TILE_GAP, -0.5, max_offset), platform_length, config_num, glm::vec3(0, 0, 1), false, BLUE_BUFF, 0));
		}
		else {
			// randomize if another tile will be spawn
			int additional_tile = coinToss(0.5);
			if (additional_tile) {
				platform.push_back(new Platform(glm::vec3(1.7 * TILE_GAP, -0.5, max_offset), platform_length, config_num, color, true, color_name, 0));
			}
		}

		platforms.push_back(platform);
		max_offset += platform_length / 2;
	}
}

void Tema2::translateSphere(float deltaTimeSeconds) {
	if (!player->isDead()) {

		// descend
		if (player->isDescending() == true) {
			player->setInAir(true);
			player->setSpherePosition(player->getPosition() - glm::vec3(0, deltaTimeSeconds * player->getSpeed(), 0));
			camera->TranslateUpword(-deltaTimeSeconds * player->getSpeed());
		}

		// stop sphere from descending
		if (player->getPosition().y <= 0) {
			player->setInAir(false);
			player->setDescend(false);
		}

		// translate forward
		camera->TranslateForward(deltaTimeSeconds * player->getSpeed());
		player->setSpherePosition(player->getPosition() - glm::vec3(0, 0, deltaTimeSeconds * player->getSpeed()));

		player->setOffset(player->getOffset() - glm::vec3(0, 0, deltaTimeSeconds * player->getSpeed()));
		player->setSphereRotation(player->getRotationAngle() - glm::vec3(deltaTimeSeconds * 10, 0, 0));

		// consume fuel
		player->setFuel(player->getFuel() - deltaTimeSeconds * player->getSpeed());

		if (player->getFuel() <= 0) {
			lives--;
			player->setFuel(MAX_FUEL);
		}

		if (lives <= 0) {
			exit(1);
		}
	}

	// render sphere 
	if (renderCameraTarget && player->getBuffTime() <= 0) {
		glm::mat4 modelMatrix(1);
		modelMatrix *= Transform3D::Translate(player->getPosition().x, player->getPosition().y, player->getPosition().z);
		modelMatrix *= Transform3D::Scale(0.5, 0.5, 0.5);
		modelMatrix *= Transform3D::RotateOX(player->getRotationAngle().x);
		modelMatrix *= Transform3D::RotateOZ(player->getRotationAngle().z);
		RenderMesh(player->getSphere(), shaders["VertexNormal"], modelMatrix);
	}
}

void Tema2::renderFuelBar() {

	float fuel_proc = player->getFuel() / (float) MAX_FUEL;

	float scale_x = window->GetResolution().x / 10.f;
	float scale_y = window->GetResolution().y / 20;

	float translate_x = window->GetResolution().x / 12;
	float translate_y = window->GetResolution().y / 9 * 8;

	// render front rectangle
	glm::mat3 modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(translate_x - (scale_x / 2) + (scale_x * fuel_proc) / 2, translate_y);
	modelMatrix *= Transform2D::Scale(scale_x * fuel_proc, scale_y);
	Tema2::RenderMesh2D(fuelBar.rectangle_front, shaders["VertexNormal"], modelMatrix);

	// render back rectangle
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(translate_x, translate_y);
	modelMatrix *= Transform2D::Scale(scale_x, scale_y);
	Tema2::RenderMesh2D(fuelBar.rectangle_back, shaders["VertexNormal"], modelMatrix);

	float x = window->GetResolution().x / 10 * 8;
	float y = window->GetResolution().y / 10 * 9;

	if (lives > 0) {
		// render back rectangle
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(x, y);
		modelMatrix *= Transform2D::Scale(0.5, 0.5);
		Tema2::RenderMesh2D(life1, shaders["VertexNormal"], modelMatrix);
	}
	
	if (lives > 1) {
		// render back rectangle
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(x + window->GetResolution().x / 20 * 1.5, y);
		modelMatrix *= Transform2D::Scale(0.5, 0.5);
		Tema2::RenderMesh2D(life2, shaders["VertexNormal"], modelMatrix);
	}
	
	if (lives > 2) {
		// render back rectangle
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(x + window->GetResolution().x / 20 * 3, y);
		modelMatrix *= Transform2D::Scale(0.5, 0.5);
		Tema2::RenderMesh2D(life3, shaders["VertexNormal"], modelMatrix);
	}
}

void Tema2::apply_buff(float deltaTimeSeconds) {
	// check buffs
	if (player->getBuffTime() > 0 && player->getBuffType() != BLUE_BUFF && player->getBuffType() != LIFE_BUFF) {

		glm::vec3 color;

		int buff_type = player->getBuffType();
		switch (buff_type) {
		case GREEN_BUFF:
			player->setFuel(min(player->getFuel() + 2 * deltaTimeSeconds * player->getSpeed(), MAX_FUEL));
			color = glm::vec3(0, 1, 0);
			break;
		case YELLOW_BUFF:
			player->setFuel(max(player->getFuel() - deltaTimeSeconds * player->getSpeed(), 0));
			color = glm::vec3(1, 1, 0);
			break;
		case ORANGE_BUFF:
			player->setSpeed(CAMERA_MAX_SPEED);
			color = glm::vec3(1, 0.6, 0);
			break;
		case RED_BUFF:
			exit(1);
			break;
		}

		player->setBuffTime(player->getBuffTime() - deltaTimeSeconds);
		if (player->getBuffTime() <= 0 && player->getBuffType() == ORANGE_BUFF) {
			player->setSpeed(CAMERA_MIN_SPEED);
		}

		glm::mat4 modelMatrix(1);
		modelMatrix *= Transform3D::Translate(player->getPosition().x, player->getPosition().y, player->getPosition().z);
		modelMatrix *= Transform3D::Scale(0.5, 0.5, 0.5);
		modelMatrix *= Transform3D::RotateOX(player->getRotationAngle().x);
		modelMatrix *= Transform3D::RotateOZ(player->getRotationAngle().z);
		RenderSimpleMesh(player->getSphere(), shaders["ShaderTema2Deformare"], modelMatrix, color);
	}

}

void Tema2::deadAnimation(float deltaTimeSeconds) {
	
		if (player->getPosition().y > -MAX_JUMP) {
			camera->TranslateUpword(-deltaTimeSeconds * 1);
			player->setSpherePosition(player->getPosition() - glm::vec3(0, deltaTimeSeconds * 1, 0));
			player->setOffset(player->getOffset() - glm::vec3(0, deltaTimeSeconds * 1, 0));
		}
		else {
			exit(1);
		}
}

void Tema2::getConfigurations()
{
	srand((time(NULL)));

	// configuration 0
	configurations.push_back(createConfiguration(0, true, true, true));

	// configuration 1
	configurations.push_back(createConfiguration(1, true, true, false));

	// configuration 2
	configurations.push_back(createConfiguration(2, false, true, true));

	// configuration 3
	configurations.push_back(createConfiguration(3, true, false, true));
	configurations[3].invalid_configurations.push_back(4);
	configurations[3].invalid_configurations.push_back(6);

	// configuration 4
	configurations.push_back(createConfiguration(4, true, false, false));
	configurations[4].invalid_configurations.push_back(6);

	// configuration 5
	configurations.push_back(createConfiguration(5, false, true, false));

	// configuration 6
	configurations.push_back(createConfiguration(6, false, false, true));
	configurations[6].invalid_configurations.push_back(4);

}

struct configuration Tema2::createConfiguration(int num, bool left, bool middle, bool right)
{
	struct configuration conf;
	conf.number = num;
	conf.left = left;
	conf.middle = middle;
	conf.right = right;
	return conf;
}

void Tema2::get_configuration_number(int& config_num, int& prev_config_num)
{
	while (1) {
		config_num = randomNum(CONFIGURATIONS_NUM + 1);

		if (config_num != 0) config_num = config_num - 1;

		if (prev_config_num == -1) return;
		int size = configurations[prev_config_num].invalid_configurations.size();
		int j;
		for (j = 0; j < size; j++) {
			if (configurations[prev_config_num].invalid_configurations[j] == config_num) {
				break;
			}
		}
		if (j == size) {
			break;
		}
	}
}


void Tema2::replace_platform(std::vector<Platform*>& platform_line, std::vector<Platform*>& platform)
{
	// position of the farthest platform
	glm::vec3 offset = platforms[platforms.size() - 1][0]->getPos();
	
	// generate a configuration number
	int config_num;
	int prev_config_num = platforms[platforms.size() - 1][0]->getConfiguration();

	get_configuration_number(config_num, prev_config_num);

	// generate platform length
	double platform_length = generatePlatformLength();

	// compute platform position
	int gap = coinToss(0.2);
	if (platforms[platforms.size() - 1][0]->getObstacleDistance() > 0) gap = 0;
	double new_offset = platforms[platforms.size() - 1][0]->getPos().z + platforms[platforms.size() - 1][0]->getLength() / 2 + platform_length / 2 + gap;
 
	// generate auxiliary_platform color
	glm::vec3 color = glm::vec3(0, 0, 1);
	int color_name = BLUE_BUFF;
	generate_color(color, color_name);

	float obstacleDistance = 0;
	
	// randomize the spawning of an obstacle
	bool obstacle = coinToss(0.15);
	if (obstacle && !gap) {
		if (!gap) {
			color = glm::vec3(0, 0, 1);
			color_name = BLUE_BUFF;
			obstacleDistance = OBSTACLE_DISTANCE;
		}
	}

	int ct = 0;
	int size = platform_line.size();
	
	// generate default_color
	glm::vec3 default_color = glm::vec3(0, 0, 1);
	int default_color_name = BLUE_BUFF;
	bool green = coinToss(0.10);

	if (green) {
		default_color = glm::vec3(0, 1, 0);
		default_color_name = GREEN_BUFF;
	}

	int sum = configurations[config_num].left + configurations[config_num].middle + configurations[config_num].right;
	if (configurations[config_num].left) {

		glm::vec3 position_offset = glm::vec3((-1.7) * TILE_GAP, -0.5, new_offset);
		modify_platform(platform_line, ct, position_offset, platform_length, config_num, default_color, default_color_name, green);

		Platform* plat = platform_line[ct];
		platform.push_back(plat);

		ct++;
	} else {
		// randomize if another tile will be spawn
		int additional_tile = coinToss(0.3);
		if (additional_tile) {

			glm::vec3 position_offset = glm::vec3((-1.7) * TILE_GAP, -0.5, new_offset);

			Platform* middle = new Platform(position_offset, platform_length, config_num, color, !obstacle, color_name, obstacleDistance);
			platform.push_back(middle);

			ct++;
			sum++;
		}
	}

	if (configurations[config_num].middle) {
		glm::vec3 position_offset = glm::vec3(0, -0.5, new_offset);
		if (ct < size) {
			modify_platform(platform_line, ct, position_offset, platform_length, config_num, default_color, default_color_name, green);
			platform.push_back(platform_line[ct]);
		}
		else {
			Platform* middle = new Platform(position_offset, platform_length, config_num, default_color, green, default_color_name);
			platform.push_back(middle);
		}
		ct++;

	} else {
		// randomize if another tile will be spawn
		int additional_tile = coinToss(0.3);
		if (additional_tile) {

			glm::vec3 position_offset = glm::vec3(0, -0.5, new_offset);

			Platform* middle = new Platform(position_offset, platform_length, config_num, color, !obstacle, color_name, obstacleDistance);
			platform.push_back(middle);
			sum++;

		}
	}

	if (configurations[config_num].right) {
		glm::vec3 position_offset = glm::vec3(1.7 * TILE_GAP, -0.5, new_offset);
		if (ct < size){
			modify_platform(platform_line, ct, position_offset, platform_length, config_num, default_color, default_color_name, green);
			platform.push_back(platform_line[ct]);
		}
		else {
			Platform* right = new Platform(position_offset, platform_length, config_num, default_color, green, default_color_name);
			platform.push_back(right);
		}
	} else {
		// randomize if another tile will be spawn
		int additional_tile = coinToss(0.3);
		if (additional_tile) {

			glm::vec3 position_offset = glm::vec3(1.7 * TILE_GAP, -0.5, new_offset);

			Platform* middle = new Platform(position_offset, platform_length, config_num, color, !obstacle, color_name, obstacleDistance);
			platform.push_back(middle);

		}
	}

}

void Tema2::modify_platform(std::vector<Platform*>& platform_line, int ct, glm::vec3 position_offset, double platform_length, int config_num, glm::vec3 color, 
	int color_name, bool special) {

	platform_line[ct]->setPosition(position_offset);
	platform_line[ct]->setLength(platform_length);
	platform_line[ct]->setConfiguration(config_num); 
	platform_line[ct]->setColor(color);
	platform_line[ct]->setSpecial(special);
	platform_line[ct]->setColorName(color_name);
	platform_line[ct]->setObstacleDistance(0);
	platform_line[ct]->setHitObstacle(false);


	if (color_name == OBSTACLE_BUFF) {
		platform_line[ct]->setObstacle();
		platform_line[ct]->setColorName(BLUE_BUFF);
		platform_line[ct]->setObstacleDistance(OBSTACLE_DISTANCE);
	}
}

void Tema2::generate_color(glm::vec3& color, int& color_name) {

	int num = randomNum(5);
	switch (num) {
	case 1:
		color = glm::vec3(1, 1, 0);
		color_name = YELLOW_BUFF;
		break;
	case 2:
		color = glm::vec3(0, 1, 0);
		color_name = GREEN_BUFF;
		break;
	case 3:
		color = glm::vec3(1, 0.6, 0);
		color_name = ORANGE_BUFF;
		break;
	case 5:
		color = glm::vec3(1, 0, 0);
		color_name = RED_BUFF;
		break;
	case 4:
		color = glm::vec3(0.9, 0, 0.3);
		color_name = LIFE_BUFF;
		break;
	}
}

float Tema2::generatePlatformLength() {
	double platform_length = randomNum(MAX_PLATFORM_LENGTH);
	if (platform_length < MIN_PLATFORM_LENGTH) {
		platform_length += MIN_PLATFORM_LENGTH - platform_length;
	}
	platform_length *= 0.7;
	return platform_length;
}

void Tema2::delete_old_platforms(std::vector<int> to_delete)
{
	int count = 0;
	if (to_delete.size() > 0) {
		int to_del = to_delete[to_delete.size() - 1];
		for (auto it = platforms.begin(); it != platforms.end(); ) {
			bool del = false;
			if (count <= to_del) {
				it = platforms.erase(it);
			}
			else {
				it++;
			}
			count++;
		}
	}
}

Mesh* Tema2::createRectangle(glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-0.5, -0.5,  0), glm::vec3(1, 1, 1), color),
		VertexFormat(glm::vec3(0.5, -0.5,  0), glm::vec3(1, 1, 1), color),
		VertexFormat(glm::vec3(-0.5, 0.5,  0), glm::vec3(1, 1, 1), color),
		VertexFormat(glm::vec3(0.5,  0.5,  0), glm::vec3(1, 1, 1), color),
	};

	std::vector<unsigned short> indices = { 0, 1, 2,		1, 3, 2, };

	return CreateMesh("rectangle", vertices, indices);
}

void Tema2::createLives() {
	std::vector<VertexFormat> vertices1;
	std::vector<unsigned short> indices1;

	std::vector<VertexFormat> vertices2;
	std::vector<unsigned short> indices2;

	std::vector<VertexFormat> vertices3;
	std::vector<unsigned short> indices3;

	float angle;
	float radius = window->GetResolution().x / 20;
	for (unsigned short i = 0; i < 1000; i++)
	{
		angle = 2 * 3.14 * i / 50;

		vertices1.emplace_back(glm::vec3(cos(angle) * radius, sin(angle) * radius, 1), glm::vec3(1, 0, 0));
		indices1.push_back(i);

		vertices2.emplace_back(glm::vec3(cos(angle) * radius, sin(angle) * radius, 1), glm::vec3(1, 0, 0));
		indices2.push_back(i);

		vertices3.emplace_back(glm::vec3(cos(angle) * radius, sin(angle) * radius, 1), glm::vec3(1, 0, 0));
		indices3.push_back(i);
	}

	vertices1.emplace_back(glm::vec3(cos(0.785) * radius, sin(0.785) * radius, 1), glm::vec3(1, 0, 0));
	indices1.push_back(50);

	vertices2.emplace_back(glm::vec3(cos(0.785) * radius, sin(0.785) * radius, 1), glm::vec3(1, 0, 0));
	indices2.push_back(50);

	vertices3.emplace_back(glm::vec3(cos(0.785) * radius, sin(0.785) * radius, 1), glm::vec3(1, 0, 0));
	indices3.push_back(50);

	life1 = new Mesh("life1");
	life1->SetDrawMode(GL_TRIANGLE_FAN);
	life1->InitFromData(vertices1, indices1);

	life2 = new Mesh("life2");
	life2->SetDrawMode(GL_TRIANGLE_FAN);
	life2->InitFromData(vertices2, indices2);

	life3 = new Mesh("life3");
	life3->SetDrawMode(GL_TRIANGLE_FAN);
	life3->InitFromData(vertices3, indices3);

	extraLife = new Mesh("extraLife");
	extraLife->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
}


Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);	// se genereaza ID-ul (numele) bufferului
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// TODO: Unbind the VAO
	glBindVertexArray(0);
	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);
	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int location4 = glGetUniformLocation(shader->program, "time");
	glUniform1f(location4, Engine::GetElapsedTime());

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::RenderMesh2D(Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	shader->Use();

	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera2D->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(orthoMatrix));

	glm::mat3 mm = modelMatrix;
	glm::mat4 model = glm::mat4(
		mm[0][0], mm[0][1], mm[0][2], 0.f,
		mm[1][0], mm[1][1], mm[1][2], 0.f,
		0.f, 0.f, mm[2][2], 0.f,
		mm[2][0], mm[2][1], 0.f, 1.f);

	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
	mesh->Render();
}


