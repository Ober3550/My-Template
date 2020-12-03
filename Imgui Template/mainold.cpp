#include "imgui.h"
#include "imgui-SFML.h"
#include "zlib.h"

#include <iostream>
#include <array>
#include <vector>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/type_ptr.hpp>

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Sphere.h"
#include "Box.h"

static int divisions = 1;
static bool wireframe = true;
static bool normalise = true;
static float scaleBox = 1.f;
static float scaleIco = 1.f;
static glm::vec3 posBox = { -2,0,0 };
static glm::vec3 posIco = { 0,0,0 };
static float min = 0.f;
static float max = 0.f;
static float rotation = 0.f;

glm::vec3 updateFacing(float pitch, float yaw) {
    glm::vec3 facing;
    facing.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    facing.y = sin(glm::radians(pitch));
    facing.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(facing);
}

int main()
{
    sf::Clock deltaClock;
    sf::Clock frameClock;

    sf::ContextSettings settings;
    settings.depthBits = 32;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "ImGui + SFML = <3", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    sf::View centreView;
    sf::Vector2u size = window.getSize();

    // Opengl Build Shaders -----------------------------

    // Opengl stuff -------------------------------------

    bool cursorGrabbed = false;
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2i windowMiddle = sf::Vector2i(windowSize.x / 2, windowSize.y / 2);
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float cameraPitch = 0.f;
    float cameraYaw = 0.f;

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);

    double frustRight = 1;
    double frustUp = frustRight * double(windowMiddle.y) / double(windowMiddle.x);
    double nearClip = 1.f;
    double fov = 70;
    double farClip = double(windowMiddle.x) / double(tan(fov * M_PI / 360));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-frustRight, frustRight, -frustUp, frustUp, nearClip, farClip);
    glMatrixMode(GL_MODELVIEW);

    GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat direction[] = { 0.0, 0.0, 1.0, 1.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 30);

    glLightfv(GL_LIGHT0, GL_AMBIENT, black);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glLightfv(GL_LIGHT0, GL_POSITION, direction);

    // Turning lights on means that polygons can't be colored
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    
    glEnable(GL_DEPTH_TEST);              // so the renderer considers depth
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    // Move the camera and face the cube
    cameraPos += glm::vec3(0.f, 2.0f, 5.0f);
    cameraPitch -= 20.f;
    cameraYaw   -= 90.f;
    cameraFront = updateFacing(cameraPitch, cameraYaw);
    float timer = 0;

    Quaternion q1;
    q1.w = 1;
    q1.i = 0;
    q1.j = 0;
    q1.k = 0;
    Quaternion q2;
    q2.w = 0;
    q2.i = 0;
    q2.j = 0;
    q2.k = 0;
    window.pushGLStates();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::E: {
                    cursorGrabbed = !cursorGrabbed;
                    sf::Mouse::setPosition(sf::Vector2i(windowMiddle.x, windowMiddle.y), window);
                }break;
                case sf::Keyboard::Escape: {
                    cursorGrabbed = !cursorGrabbed;
                    sf::Mouse::setPosition(sf::Vector2i(windowMiddle.x, windowMiddle.y), window);
                }break;
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                if (cursorGrabbed)
                {
                    sf::Mouse::setPosition(sf::Vector2i(windowMiddle.x, windowMiddle.y), window);
                    if (event.mouseMove.x != windowMiddle.x || event.mouseMove.y != windowMiddle.y)
                    {
                        const float sensitivity = 0.1f;
                        cameraYaw += (event.mouseMove.x - windowMiddle.x) * sensitivity;
                        cameraPitch += (event.mouseMove.y - windowMiddle.y) * -sensitivity;
                        if (cameraYaw > 180.f)
                            cameraYaw = -180.f;
                        if (cameraYaw < -180.f)
                            cameraYaw = 180.f;
                        if (cameraPitch > 89.0f)
                            cameraPitch = 89.0f;
                        if (cameraPitch < -89.0f)
                            cameraPitch = -89.0f;
                        cameraFront = updateFacing(cameraPitch, cameraYaw);
                    }
                }
            }
            else if (event.type == sf::Event::Resized)
            {
                windowSize = window.getSize();
                windowMiddle = sf::Vector2i(windowSize.x / 2, windowSize.y / 2);
                double ratio = double(windowSize.x) / double(windowSize.y);
                window.popGLStates();
                glViewport(0, 0, windowSize.x, windowSize.y);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glFrustum(-frustRight * ratio, frustRight * ratio, -frustUp, frustUp, nearClip, farClip);
                glMatrixMode(GL_MODELVIEW);
                window.pushGLStates();
            }
            else if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        
        // Movement code
        const float cameraSpeed = 0.05f; // adjust accordingly
        float accelerate = (1.f + float(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)));
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            cameraPos += cameraSpeed * cameraFront * accelerate;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            cameraPos -= cameraSpeed * cameraFront * accelerate;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * accelerate;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * accelerate;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            cameraPos += glm::vec3(0.f, 1.f, 0.f) * cameraSpeed * accelerate;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            cameraPos -= glm::vec3(0.f, 1.f, 0.f) * cameraSpeed * accelerate;
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::ShowDemoWindow();
        ImGui::Begin("Box");
        ImGui::SliderFloat("Box: x", &posBox.x, -10, 10);
        ImGui::SliderFloat("Box: y", &posBox.y, -10, 10);
        ImGui::SliderFloat("Box: z", &posBox.z, -10, 10);
        ImGui::SliderFloat("Scale Box", &scaleBox, 0.1, 10);
        ImGui::End();

        ImGui::Begin("Icosahedron");
        ImGui::SliderFloat("Ico: x", &posIco.x, -10, 10);
        ImGui::SliderFloat("Ico: y", &posIco.y, -10, 10);
        ImGui::SliderFloat("Ico: z", &posIco.z, -10, 10);
        ImGui::SliderFloat("Rotate", &rotation, -M_PI, M_PI);
        //ImGui::SliderFloat("w", &q1.w, -1, 1);
        ImGui::SliderFloat("i", &q1.i, -1, 1);
        ImGui::SliderFloat("j", &q1.j, -1, 1);
        ImGui::SliderFloat("k", &q1.k, -1, 1);
        EulerAngles euler = ToEulerAngles(q1.mul(rotation));
        ImGui::Text(std::string("Pitch: " + std::to_string(euler.pitch)).c_str());
        ImGui::Text(std::string("Roll:  " + std::to_string(euler.roll)).c_str());
        ImGui::Text(std::string("Yaw:   " + std::to_string(euler.yaw)).c_str());
        ImGui::SliderFloat("Scale Ico", &scaleIco, 0.1, 10);
        ImGui::SliderInt("Divisions", &divisions, 1, 50);
        ImGui::Checkbox("Wireframe", &wireframe);
        ImGui::Checkbox("Normalise", &normalise);
        ImGui::Text(std::string("Min: "+std::to_string(min)+" Max: "+std::to_string(max)).c_str());
        ImGui::Text(std::string("Proportion: "+std::to_string(max/min)).c_str());
        ImGui::End();
        q1 = q1 / Quaternion::abs(q1);
        
        timer++;
        window.clear();
        {
            window.popGLStates();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glLoadMatrixf(glm::value_ptr(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));

            //if (false)
            {
                // Draw a white grid "floor" for the tetrahedron to sit on.
                glColor3f(1.0, 1.0, 1.0);
                glBegin(GL_LINES);
                const float lines = 50;
                const float gap = 0.5;
                float dist = lines * gap * 0.5;
                for (GLfloat i = -dist; i <= dist; i += gap) {
                    glNormal3f(0.f, 1.f, 0.f);
                    glVertex3f(i, 0, dist); glVertex3f(i, 0, -dist);
                    glNormal3f(0.f, 1.f, 0.f);
                    glVertex3f(dist, 0, i); glVertex3f(-dist, 0, i);
                }
                glEnd();
            }

            // Draw a box
            //drawBox(posBox, glm::vec3(0, -timer, 0), scaleBox, wireframe);
            drawIco(posIco, q1.mul(rotation), scaleIco, divisions, wireframe, normalise);
            glCullFace(GL_FRONT);
            glFlush();
            window.pushGLStates();
        }
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}