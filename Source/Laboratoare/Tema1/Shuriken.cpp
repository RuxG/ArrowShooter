#include "Shuriken.h"


Shuriken::Shuriken(glm::vec3 center, glm::vec3 color)
{
    triangle1 = CreateTriangle("t1", center, 1, color);
    triangle2 = CreateTriangle("t2", center, 2, color);
    triangle3 = CreateTriangle("t3", center, 3, color);
    triangle4 = CreateTriangle("t4", center, 4, color);

    this->center = center;

    this->circleX = center.x;
    this->circleY = center.y;
    this->radius = sqrt(pow(TRIANGLE_LENGTH, 2) + pow(TRIANGLE_LENGTH, 2));

    this->scale = 1;
}


Mesh* Shuriken::CreateTriangle(std::string name, glm::vec3 position, int number, glm::vec3 color)
{
    Mesh* triangle = new Mesh("triangle");
    triangle->SetDrawMode(GL_TRIANGLES);

    std::vector<VertexFormat> vertices;


    vertices.push_back(VertexFormat(position));

    switch (number) {
    case 1:
        vertices.push_back(VertexFormat(position + glm::vec3(0, TRIANGLE_LENGTH, 0), glm::vec3(1, 0, 1)));
        vertices.push_back(VertexFormat(position + glm::vec3((-1) * TRIANGLE_LENGTH, TRIANGLE_LENGTH, 0), glm::vec3(1, 0, 1)));
        break;
    case 2:
        vertices.push_back(VertexFormat(position + glm::vec3((-1) * TRIANGLE_LENGTH, 0, 0), glm::vec3(1, 0, 1)));
        vertices.push_back(VertexFormat(position + glm::vec3((-1) * TRIANGLE_LENGTH, (-1) * TRIANGLE_LENGTH, 0), glm::vec3(1, 0, 1)));
        break;
    case 3:
        vertices.push_back(VertexFormat(position + glm::vec3(0, (-1) * TRIANGLE_LENGTH, 0), glm::vec3(1, 0, 1)));
        vertices.push_back(VertexFormat(position + glm::vec3(TRIANGLE_LENGTH, (-1) * TRIANGLE_LENGTH, 0), glm::vec3(1, 0, 1)));
        break;
    case 4:
        vertices.push_back(VertexFormat(position + glm::vec3(TRIANGLE_LENGTH, 0, 0), glm::vec3(1, 0, 1)));
        vertices.push_back(VertexFormat(position + glm::vec3(TRIANGLE_LENGTH, TRIANGLE_LENGTH, 0), glm::vec3(1, 0, 1)));
        break;
    default:
        break;
    }

    std::vector<unsigned short> indices = {
        0, 1, 2,
    };

    triangle->InitFromData(vertices, indices);

    return triangle;
}

Shuriken::~Shuriken()
{
}

Mesh* Shuriken::getTriangle1()
{
    return triangle1;
}

Mesh* Shuriken::getTriangle2()
{
    return triangle2;
}

Mesh* Shuriken::getTriangle3()
{
    return triangle3;
}

Mesh* Shuriken::getTriangle4()
{
    return triangle4;
}

glm::vec3 Shuriken::getCenter()
{
    return center;
}


int Shuriken::getTranslateOX()
{
    return translateOX;
}

void Shuriken::setCenter(glm::vec3 position)
{
    this->center = position;
}

void Shuriken::setTranslateOX(int x)
{
    this->translateOX = x;
}

int Shuriken::getCircleX()
{
    return circleX;
}

int Shuriken::getCircleY()
{
    return circleY;
}

int Shuriken::getRadius()
{
    return radius;
}

float Shuriken::getScale()
{
    return scale;
}

bool Shuriken::isDestroyed()
{
    return destroyed;
}

void Shuriken::setCircleX(int x)
{
    circleX = x;
}

void Shuriken::setCircleY(int y)
{
    circleY = y;
}


void Shuriken::setRadius(int r)
{
    radius = r;
}

void Shuriken::setDestroyed(bool destroyed)
{
    this->destroyed = destroyed;
}

void Shuriken::setScale(float scale)
{
    this->scale = scale;
}