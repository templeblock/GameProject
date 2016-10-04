#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ICamera.hpp"

#include <glm\glm.hpp>

class Camera : public ICamera
{
public:

	virtual void init();
	virtual void release();

	virtual float* getViewMatrix();
	virtual float* getPerspectiveMatrix();

//private:
	glm::mat4 viewMatrix;
	glm::mat4 perspective;

};

#endif