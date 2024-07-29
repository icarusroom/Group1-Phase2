#include "MyCamera.h"
#include <iostream>

// Constructor to initialize the camera's position, orientation, and up vector
MyCamera::MyCamera()
{
    this->cameraPos = glm::vec3(0.0f, 0.0f, 0.0f); // Initial position
    this->orientation = glm::vec3(0.0f, 0.0f, -1.0f); // Initial orientation (looking down the negative Z-axis)
    this->up = glm::normalize(glm::vec3(0.f, 1.f, 0.f)); // Up vector (normalized)

    SetViewMatrix(); // Set the initial view matrix
}

// Function to move the camera by a specified vector
void MyCamera::CameraMovement(glm::vec3 movement)
{
    this->cameraPos += movement; // Update camera position
    SetViewMatrix(); // Update the view matrix
}

// Function to set the camera's position
void MyCamera::setCameraPosition(glm::vec3 position)
{
    this->cameraPos = position; // Update the camera position
    SetViewMatrix(); // Update the view matrix
}

// Function to set the camera's orientation
void MyCamera::setCenter(glm::vec3 orientation)
{
    this->orientation = orientation; // Update the camera orientation
    SetViewMatrix(); // Update the view matrix
}

// Function to update the camera's position and orientation based on user input
void MyCamera::Update(GLFWwindow* window, float time)
{
    float cameraSpeed = 2.5f * time; // Calculate the camera speed
    glm::vec3 lookAtPoint(0.0f, 0.0f, 0.0f); // Point of interest (0, 0, 0)

    // Rotate the camera around the point of interest (lookAtPoint)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // Rotate left around the lookAtPoint
        glm::vec3 toCamera = cameraPos - lookAtPoint;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), cameraSpeed, up);
        cameraPos = lookAtPoint + glm::vec3(rotation * glm::vec4(toCamera, 1.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        // Rotate right around the lookAtPoint
        glm::vec3 toCamera = cameraPos - lookAtPoint;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -cameraSpeed, up);
        cameraPos = lookAtPoint + glm::vec3(rotation * glm::vec4(toCamera, 1.0f));
    }

    // Move the camera forward or backward relative to its orientation around the lookAtPoint
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(lookAtPoint - cameraPos);
        glm::vec3 rotationAxis = (glm::abs(glm::dot(direction, up)) < 0.99f) ? glm::cross(direction, up) : glm::vec3(1.0f, 0.0f, 0.0f);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -cameraSpeed, rotationAxis);
        cameraPos = lookAtPoint + glm::vec3(rotation * glm::vec4(cameraPos - lookAtPoint, 1.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(lookAtPoint - cameraPos);
        glm::vec3 rotationAxis = (glm::abs(glm::dot(direction, up)) < 0.99f) ? glm::cross(direction, up) : glm::vec3(1.0f, 0.0f, 0.0f);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), cameraSpeed, rotationAxis);
        cameraPos = lookAtPoint + glm::vec3(rotation * glm::vec4(cameraPos - lookAtPoint, 1.0f));
    }

    // Update the view matrix
    SetViewMatrix();
}

// Function to set the view matrix based on the camera's position and orientation
void MyCamera::SetViewMatrix()
{
    this->view_matrix = glm::lookAt(this->cameraPos, this->orientation, up);
}
