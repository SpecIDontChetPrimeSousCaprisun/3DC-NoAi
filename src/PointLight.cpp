#include "PointLight.hpp"
#include "Window.hpp"

PointLight PointLight::lights[8];

void PointLight::init() {
    glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f),
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    for (int i = 0; i < 8; i++) {
	lights[i].setParent(Window::parent);
	lights[i].position = pointLightPositions[i];
    }
}
