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

void drawBox(bool wire, bool filled) {
    Mesh box;
    box.triangles = box_face;
    box.vertices = box_vert;
    if (filled) {
        glColor3f(1.0, 1.0, 1.0);
        box.drawFilled();
    }
    if (wire) {
        glColor3f(0.f, 0.f, 0.f);
        glScalef(1.01f, 1.01f, 1.01f);
        box.drawWireFrame();
    }
}