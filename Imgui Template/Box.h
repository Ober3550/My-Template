#pragma once
#include "Mesh.h"

const std::vector<glm::vec3> box_vert = { {
    {-1.f,-1.f,-1.f},
    {-1.f,1.f,-1.f},
    {-1.f,1.f,1.f},
    {-1.f,-1.f,1.f},
    {1.f,-1.f,1.f},
    {1.f,1.f,1.f},
    {1.f,1.f,-1.f},
    {1.f,-1.f,-1.f}
} };
const std::vector<std::array<int, 3>> box_face = { {
    {0,1,2},
    {2,3,0},
    {5,4,3},
    {3,2,5},
    {4,5,6},
    {6,7,4},
    {1,0,7},
    {7,6,1},
    {0,3,4},
    {4,7,0},
    {6,5,2},
    {2,1,6}
} };

void drawBox(glm::vec3 pos, glm::vec3 rot, float scale, bool wire, bool filled = true) {
    Mesh box;
    box.pos = pos;
    box.rotation = rot;
    box.scale = scale;
    box.triangles = box_face;
    box.vertices = box_vert;
    if (filled) {
        glColor3f(1.0, 1.0, 1.0);
        box.drawFilled();
    }
    if (wire) {
        glColor3f(0.f, 0.f, 0.f);
        box.drawWireFrame();
    }
}

void drawBox(glm::vec3 pos, Quaternion rot, float scale, bool wire, bool filled = true) {
    EulerAngles angles = ToEulerAngles(rot);
    glm::vec3 rotation;
    rotation.x = angles.pitch;
    rotation.y = angles.roll;
    rotation.z = angles.yaw;
    drawBox(pos, rotation, scale, wire, filled);
}