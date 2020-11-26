#pragma once
#include "Mesh.h"


Mesh divideFace(std::vector<glm::vec3>& vertices, std::vector<std::array<int, 3>>& triangles, int idx, int div) {
    std::vector<glm::vec3> currentVerts;
    std::vector<glm::vec3> newVerts;
    std::vector<std::array<int, 3>>   newTris;
    currentVerts.push_back(vertices[triangles[idx][0]]);
    currentVerts.push_back(vertices[triangles[idx][1]]);
    currentVerts.push_back(vertices[triangles[idx][2]]);
    glm::vec3 edgeA = currentVerts[1] - currentVerts[0];
    glm::vec3 edgeB = currentVerts[2] - currentVerts[0];
    int prevRow = 0;
    int row = 0;
    for (int i = 0; i < (div + 1); i++) {
        glm::vec3 leftVert = currentVerts[0] + float(i) * (edgeA / float(div));
        glm::vec3 rightVert = currentVerts[0] + float(i) * (edgeB / float(div));
        glm::vec3 horizontalEdge = rightVert - leftVert;
        prevRow = row;
        row = newVerts.size();
        for (int j = 0; j < i; j++) {
            glm::vec3 middleVert = leftVert + float(j) * (horizontalEdge / float(i));
            newVerts.push_back(middleVert);
            // Upper triangles
            newTris.push_back({ row + j,row + 1 + j,prevRow + j });
            // Lower triangles
            if (j != i - 1)
                newTris.push_back({ prevRow + j, row + j + 1, prevRow + j + 1 });
        }
        newVerts.push_back(rightVert);
    }
    int triOffset = vertices.size();
    Mesh newMesh;
    newMesh.triangles = newTris;
    newMesh.vertices = newVerts;
    return newMesh;
}
void normaliseVerts(std::vector<glm::vec3>& vertices) {
    for (int i = 0; i < vertices.size(); i++) {
        float mag = sqrt(pow(vertices[i].x, 2) + pow(vertices[i].y, 2) + pow(vertices[i].z, 2));
        vertices[i] /= mag;
    }
}
#define GR 1.61803398875
std::vector<glm::vec3> ico_vert = {
        { 1.f,  GR, 0.f}, // 0
        {-1.f,  GR, 0.f}, // 1
        { 1.f, -GR, 0.f}, // 2
        {-1.f, -GR, 0.f}, // 3
        { GR, 0.f,  1.f}, // 4
        { GR, 0.f, -1.f}, // 5
        {-GR, 0.f,  1.f}, // 6
        {-GR, 0.f, -1.f}, // 7
        { 0.f, 1.f,  GR}, // 8
        { 0.f,-1.f,  GR}, // 9
        { 0.f, 1.f, -GR}, // 10
        { 0.f,-1.f, -GR}, // 11
};
std::vector<std::array<int, 3>> ico_face = {
    {5,4,0},
    {1,6,7},
    {2,4,5},
    {7,6,3},
    {9,8,4},
    {5,10,11},
    {6,8,9},
    {11,10,7},
    {1,0,8},
    {9,2,3},
    {10,0,1},
    {3,2,11},
    {0,4,8},
    {10,5,0},
    {1,8,6},
    {1,7,10},
    {2,9,4},
    {5,11,2},
    {3,6,9},
    {3,11,7},
};
void drawIco(glm::vec3 pos, glm::vec3 rot, float scale, int divisions, bool wireframe = true, bool normalise = false) {
    Mesh newMesh;
    newMesh.pos = pos;
    newMesh.rotation = rot;
    newMesh.scale = scale;
    if (divisions > 1) {
        for (int i = 0; i < 20; i++) {
            newMesh = newMesh + divideFace(ico_vert, ico_face, i, divisions);
        }
    }
    else
    {
        newMesh.vertices = ico_vert;
        newMesh.triangles = ico_face;
    }
    if (normalise) {
        normaliseVerts(newMesh.vertices);
        float bonus_scale = sqrt(pow(GR, 2) + 1);
        newMesh.scale *= bonus_scale;
    }
    glColor3f(1.f, 1.f, 1.f);
    newMesh.drawFilled();
    if (wireframe) {
        glColor3f(0.f, 0.f, 0.f);
        newMesh.drawWireFrame();
    }
}
void drawSphere(glm::vec3 pos, float scale, int divisions = 10, bool wireframe = false) {
    drawIco(pos, glm::vec3(0,0,0), scale, divisions, wireframe, true);
}