#include <iostream>
#include <string>
#include <chrono>
#include <limits>
#include <vector>
#include <iomanip>
#include <cmath>
#include "GameObject.h"

#include "P6/PhysicsWorld.h"
#include "P6/RenderParticle.h"
#include "P6/ParticleContact.h"
#include "Camera/OrthoCamera.h"
#include "Camera/PerspectiveCamera.h"
#include "P6/Force/Chain.h"
#include "RenderLine.h"

using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(16ms); // Define a constant time step for simulation

enum class CameraMode {
    Orthographic,
    Perspective
};

int main(void)
{
    // User inputs value
    float cableLength, particleGap, particleRad, gravStrength, forceX, forceY, forceZ;
    std::cout << "Cable Length: "; std::cin >> cableLength;
    std::cout << "Particle Gap: "; std::cin >> particleGap;
    std::cout << "Particle Radius: "; std::cin >> particleRad;
    std::cout << "Gravity Strength: "; std::cin >> gravStrength;
    std::cout << "Apply Force" << std::endl;
    std::cout << "x: "; std::cin >> forceX;
    std::cout << "y: "; std::cin >> forceY;
    std::cout << "z: "; std::cin >> forceZ;

    GLFWwindow* window;
    float window_width = 800;
    float window_height = 800;

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    if (!glfwInit()) // Initialize the GLFW library
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(window_width, window_height, "GDPHYSX Phase 2 Grp 1", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(); // Load OpenGL functions

    // Initialize OrthoCamera and set its position
    auto ortho_camera = new OrthoCamera();
    ortho_camera->setCameraPosition(glm::vec3(0.0f, 0.0f, 400.0f));

    // Initialize PerspectiveCamera and set its position
    auto pers_camera = new PerspectiveCamera();
    pers_camera->setCameraPosition(glm::vec3(0, 0.f, 550.f));

    // Initialize PhysicsWorld with gravity strength
    auto pWorld = physics::PhysicsWorld(MyVector(0, gravStrength, 0));

    // Load sphere model
    auto sphere = GameObject("3D/sphere.obj", "Shaders/sample.vert", "Shaders/sample.frag");

    glViewport(0, 0, 800, 800); // Set the viewport

    // Initialize RenderParticles list
    std::list<RenderParticle*> RenderParticles;

    // Spawning Variables
    float fThreshHold = 0.2f;
    float fTicks = 0.0f;

    // Default CameraMode
    CameraMode currentCameraMode = CameraMode::Orthographic;

    // Matrices
    glm::mat4 identity_matrix = glm::mat4(1.0f);
    glm::mat4 projection_matrix = glm::mat4(1.f);
    glm::mat4 view_matrix = glm::mat4(1.0f);

    // Pause/play flag
    bool paused = false;
    bool pressed = false;

    // Instantiate PhysicsParticles and add them to the PhysicsWorld
    physics::PhysicsParticle p3 = physics::PhysicsParticle();
    p3.Position = physics::MyVector(0, 100, 0);
    p3.mass = 50;
    p3.radius = particleRad;
    pWorld.AddParticle(&p3);

    physics::PhysicsParticle p2 = physics::PhysicsParticle();
    p2.Position = physics::MyVector(p3.Position.x - particleGap, 100, 0);
    p2.mass = 50;
    p2.radius = particleRad;
    pWorld.AddParticle(&p2);

    physics::PhysicsParticle p1 = physics::PhysicsParticle();
    p1.Position = physics::MyVector(p2.Position.x - particleGap, 100, 0);
    p1.mass = 50;
    p1.radius = particleRad;
    pWorld.AddParticle(&p1);

    physics::PhysicsParticle p4 = physics::PhysicsParticle();
    p4.Position = physics::MyVector(p3.Position.x + particleGap, 100, 0);
    p4.mass = 50;
    p4.radius = particleRad;
    pWorld.AddParticle(&p4);

    physics::PhysicsParticle p5 = physics::PhysicsParticle();
    p5.Position = physics::MyVector(p4.Position.x + particleGap, 100, 0);
    p5.mass = 50;
    p5.radius = particleRad;
    pWorld.AddParticle(&p5);

    // Create Chain springs and add them to the force registry
    physics::MyVector springPos3 = physics::MyVector(0, 150, 0);
    physics::Chain chainSpring3 = Chain(springPos3, 0, cableLength);
    pWorld.forceRegistry.Add(&p3, &chainSpring3);

    physics::MyVector springPos2 = physics::MyVector(springPos3.x - particleGap, 150, 0);
    physics::Chain chainSpring2 = Chain(springPos2, 0, cableLength);
    pWorld.forceRegistry.Add(&p2, &chainSpring2);

    physics::MyVector springPos1 = physics::MyVector(springPos2.x - particleGap, 150, 0);
    physics::Chain chainSpring1 = Chain(springPos1, 0, cableLength);
    pWorld.forceRegistry.Add(&p1, &chainSpring1);

    physics::MyVector springPos4 = physics::MyVector(springPos3.x + particleGap, 150, 0);
    physics::Chain chainSpring4 = Chain(springPos4, 0, cableLength);
    pWorld.forceRegistry.Add(&p4, &chainSpring4);

    physics::MyVector springPos5 = physics::MyVector(springPos4.x + particleGap, 150, 0);
    physics::Chain chainSpring5 = Chain(springPos5, 0, cableLength);
    pWorld.forceRegistry.Add(&p5, &chainSpring5);

    // Create RenderParticles and add them to the list
    RenderParticle rp1 = RenderParticle(&p1, &sphere, glm::vec4(0.4f, 0, 0, 0.f), p1.radius);
    RenderParticles.push_back(&rp1);

    RenderParticle rp2 = RenderParticle(&p2, &sphere, glm::vec4(0.4f, 0, 0, 0.f), p2.radius);
    RenderParticles.push_back(&rp2);

    RenderParticle rp3 = RenderParticle(&p3, &sphere, glm::vec4(0.4f, 0, 0, 0.f), p3.radius);
    RenderParticles.push_back(&rp3);

    RenderParticle rp4 = RenderParticle(&p4, &sphere, glm::vec4(0.4f, 0, 0, 0.f), p4.radius);
    RenderParticles.push_back(&rp4);

    RenderParticle rp5 = RenderParticle(&p5, &sphere, glm::vec4(0.4f, 0, 0, 0.f), p5.radius);
    RenderParticles.push_back(&rp5);

    // Create RenderLines to visualize the springs
    RenderLine line1 = RenderLine(springPos1, p1.Position, glm::vec4(1, 1, 1, 1));
    RenderLine line2 = RenderLine(springPos2, p2.Position, glm::vec4(1, 1, 1, 1));
    RenderLine line3 = RenderLine(springPos3, p3.Position, glm::vec4(1, 1, 1, 1));
    RenderLine line4 = RenderLine(springPos4, p4.Position, glm::vec4(1, 1, 1, 1));
    RenderLine line5 = RenderLine(springPos5, p5.Position, glm::vec4(1, 1, 1, 1));

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);
        prev_time = curr_time;

        curr_ns += dur;

        // Simulation update loop
        if (curr_ns >= timestep) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            curr_ns -= curr_ns;

            // Key inputs for camera mode switching and applying force
            if (glfwGetKey(window, GLFW_KEY_1))
            {
                currentCameraMode = CameraMode::Orthographic;
            }
            if (glfwGetKey(window, GLFW_KEY_2))
            {
                currentCameraMode = CameraMode::Perspective;
            }
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !pressed)
            {
                p1.AddForce(physics::MyVector(forceX, forceY, forceZ));
                pressed = true;
            }
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
            {
                pressed = false;
            }

            // Update cameras based on mode
            if (currentCameraMode == CameraMode::Orthographic) {
                projection_matrix = ortho_camera->getViewProjection();
                ortho_camera->Update(window, (float)ms.count() / 1000);
                view_matrix = ortho_camera->GetViewMatrix();
            }
            else if (currentCameraMode == CameraMode::Perspective) {
                projection_matrix = pers_camera->getViewProjection();
                pers_camera->Update(window, (float)ms.count() / 1000);
                view_matrix = pers_camera->GetViewMatrix();
            }

            if (!paused) {
                pWorld.Update((float)ms.count() / 1000); // Update the PhysicsWorld

            }
        }

        // Render particles
        for (std::list<RenderParticle*>::iterator i = RenderParticles.begin(); i != RenderParticles.end(); i++) {
            (*i)->Draw(identity_matrix, projection_matrix, view_matrix);
        }

        // Render lines
        line1.Update(springPos1, p1.Position, projection_matrix, view_matrix);
        line1.Draw();

        line2.Update(springPos2, p2.Position, projection_matrix, view_matrix);
        line2.Draw();

        line3.Update(springPos3, p3.Position, projection_matrix, view_matrix);
        line3.Draw();

        line4.Update(springPos4, p4.Position, projection_matrix, view_matrix);
        line4.Draw();

        line5.Update(springPos5, p5.Position, projection_matrix, view_matrix);
        line5.Draw();

        glfwSwapBuffers(window); // Swap front and back buffers
        glfwPollEvents(); // Poll for and process events
    }

    glfwTerminate(); // Terminate GLFW
    return 0;
}
