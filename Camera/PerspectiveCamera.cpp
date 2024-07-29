#include "PerspectiveCamera.h"
#include <iostream>

PerspectiveCamera::PerspectiveCamera() : MyCamera()
{
	this->window_height = 700;
	this->window_width = 700;
	this->theta = 45.5f;
	this->phi = 89.5f;
	this->aspectRatio = window_height / window_width;
	this->SetProjectionMatrix(60.f);
	viewMatrix = glm::mat4(1.0f); // Initialize as identity matrix
}

glm::mat4 PerspectiveCamera::getViewProjection() const
{
	return projectionMatrix * viewMatrix;
}

void PerspectiveCamera::SetProjectionMatrix(float viewAngle)
{
	//dont edit anymore
	this->projectionMatrix = glm::perspective(glm::radians(viewAngle), this->aspectRatio, 0.1f, 500.0f);
}






