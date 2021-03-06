
/// Internal Includes
#include "Camera.hpp"

/// External Includes
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

/// Std Includes

//@ TODO remove using
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

Camera::Camera() : viewMatrix(mat4()), perspective(mat4()) {}

Camera::~Camera() {}

float * Camera::getViewMatrix() {
	return &viewMatrix[0][0];
}

float * Camera::getPerspectiveMatrix() {
	return &perspective[0][0];
}

float * Camera::getOrthoMatrix() {
	return &orthoMatrix[0][0];
}

glm::mat4 Camera::viewProjection() const {
	return perspective * viewMatrix;
}

glm::vec3 Camera::calculateMouseRay(float x, float y, IRenderEngine* re) {
	re->toNormalizedDeviceSpace(x, y);

	vec4 clipSpace(x, y, -1.0f, 1.0f);
	mat4 invPerspective = glm::inverse(perspective);

	vec4 eyeSpace = invPerspective * clipSpace;
	eyeSpace.z = -1.0;
	eyeSpace.w = 0.0f;

	mat4 invView = glm::inverse(viewMatrix);
	vec4 ray = invView * eyeSpace;

	return glm::normalize(vec3(ray));
}

// @todo optimize to a calc inverse that runs at start of frame
glm::vec3 Camera::getPos() const {
	glm::mat4 view = glm::inverse(viewMatrix);

	float x = view[3][0];
	float y = view[3][1];
	float z = view[3][2];

	return glm::vec3(x, y, z);
}