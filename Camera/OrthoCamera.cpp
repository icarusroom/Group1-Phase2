#include "OrthoCamera.h"

// Constructor for the OrthoCamera class, which initializes the orthographic projection
OrthoCamera::OrthoCamera() : MyCamera()  // Call the base class constructor
{
    // Set the orthographic projection matrix
    this->projectionMatrix = glm::ortho(-400.f, 400.f, -400.f, 400.f, -400.f, 800.f);
    // Set the view matrix to the identity matrix
    this->viewMatrix = glm::mat4(1.0f);
}

// Function to get the combined view and projection matrix
glm::mat4 OrthoCamera::getViewProjection() const
{
    // Return the product of the projection and view matrices
    return projectionMatrix * viewMatrix;
}
