#include "PerspectiveCamera.h"
#include <iostream>

// Constructor for the PerspectiveCamera class, which initializes the perspective projection
PerspectiveCamera::PerspectiveCamera() : MyCamera()  // Call the base class constructor
{
    // Initialize window dimensions
    this->window_height = 800;
    this->window_width = 800;

    // Initialize camera angles
    this->theta = 45.5f;
    this->phi = 89.5f;

    // Calculate aspect ratio
    this->aspectRatio = static_cast<float>(window_height) / static_cast<float>(window_width);

    // Set the projection matrix with a view angle of 45 degrees
    this->SetProjectionMatrix(45.0f);

    // Initialize the view matrix to the identity matrix
    viewMatrix = glm::mat4(1.0f);
}

// Function to get the combined view and projection matrix
glm::mat4 PerspectiveCamera::getViewProjection() const
{
    // Return the product of the projection and view matrices
    return projectionMatrix * viewMatrix;
}

// Function to set the projection matrix based on the given view angle
void PerspectiveCamera::SetProjectionMatrix(float viewAngle)
{
    // Set the perspective projection matrix using the given view angle, aspect ratio, near, and far planes
    this->projectionMatrix = glm::perspective(glm::radians(viewAngle), this->aspectRatio, 0.1f, 2000.0f);
}
