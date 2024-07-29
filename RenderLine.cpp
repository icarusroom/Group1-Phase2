#include "RenderLine.h"

// Update the line's end points and the matrices for projection and view
void RenderLine::Update(physics::MyVector p1, physics::MyVector p2, glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    // Update the endpoints of the line
    P1 = p1;
    P2 = p2;
    // Update the projection and view matrices
    this->ProjectionMatrix = projectionMatrix;
    this->ViewMatrix = viewMatrix;
}

// Draw the line using OpenGL
void RenderLine::Draw()
{
    glUseProgram(0); // Use the default shader program

    // Transform the endpoints to clip space
    glm::vec4 d1 = this->ProjectionMatrix * this->ViewMatrix * glm::vec4(P1.x, P1.y, P1.z, 1.0f);
    glm::vec4 d2 = this->ProjectionMatrix * this->ViewMatrix * glm::vec4(P2.x, P2.y, P2.z, 1.0f);

    // Normalize the endpoints to be within the [-1, 1] range
    d1 = normalizeByMaxComponent(d1);
    d2 = normalizeByMaxComponent(d2);

    // Draw the line using OpenGL
    glBegin(GL_LINES);
    glVertex3f(d1.x, d1.y, d1.z);
    glVertex3f(d2.x, d2.y, d2.z);
    glEnd();
}

// Helper function to normalize a vector by its maximum component
glm::vec4 RenderLine::normalizeByMaxComponent(const glm::vec4& vec) {
    // Find the maximum component among x, y, z, and w
    float maxComponent = std::max({ vec.x, vec.y, vec.z, vec.w });
    // Divide all components by the maximum component to normalize
    return vec / maxComponent;
}
