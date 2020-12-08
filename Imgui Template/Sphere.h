#pragma once
#include "Mesh.h"


#define GR 1.61803398875
#define WR 0.18181818181 // The proportion of the width for 10 triangles across
#define HR 0.33333333333 // The proportion of the height for 3 bands of triangles
float aoff = .0f;
float boff = .5f;
float toff = -.5f;
float uoff = 1.f;
std::vector<glm::vec3> ico_vert = {
       { 1.f,  GR, 0.f},    // 0
       {-1.f,  GR, 0.f},    // 1
       { 1.f, -GR, 0.f},    // 2
       {-1.f, -GR, 0.f},    // 3
       { GR, 0.f,  1.f},    // 4
       { GR, 0.f, -1.f},    // 5
       {-GR, 0.f,  1.f},    // 6
       {-GR, 0.f, -1.f},    // 7
       { 0.f, 1.f,  GR},    // 8
       { 0.f,-1.f,  GR},    // 9
       { 0.f, 1.f, -GR},    // 10
       { 0.f,-1.f, -GR},    // 11
       // Belt wrap around
       {-1.f,  GR, 0.f},    // 12
       {-GR,  0.f, 1.f},    // 13
       // Bottom anti-dupe
       { 1.f,  GR, 0.f},    // 14
       { 1.f,  GR, 0.f},    // 15
       { 1.f,  GR, 0.f},    // 16
       { 1.f,  GR, 0.f},    // 17
       // Top anti-dupe
       {-1.f, -GR, 0.f},    // 18
       {-1.f, -GR, 0.f},    // 19
       {-1.f, -GR, 0.f},    // 20
       {-1.f, -GR, 0.f},    // 21
};
std::vector<glm::vec2> ico_uvs = {
    {(4 + uoff) * WR, 0 * HR},  // 0
    {(5 + boff) * WR, 1 * HR},  // 1
    {(3 + aoff) * WR, 2 * HR},  // 2
    {(5 + toff) * WR, 3 * HR},  // 3
    {(3 + boff) * WR, 1 * HR},  // 4
    {(2 + boff) * WR, 1 * HR},  // 5
    {(5 + aoff) * WR, 2 * HR},  // 6
    {(1 + aoff) * WR, 2 * HR},  // 7
    {(4 + boff) * WR, 1 * HR},  // 8
    {(4 + aoff) * WR, 2 * HR},  // 9
    {(1 + boff) * WR, 1 * HR},  // 10
    {(2 + aoff) * WR, 2 * HR},  // 11
    {(0 + boff) * WR, 1 * HR},  // 12
    {(0 + aoff) * WR, 2 * HR},  // 13
    {(3 + uoff) * WR, 0 * HR},  // 14
    {(2 + uoff) * WR, 0 * HR},  // 15
    {(1 + uoff) * WR, 0 * HR},  // 16
    {(0 + uoff) * WR, 0 * HR},  // 17
    {(4 + toff) * WR, 3 * HR},  // 18
    {(3 + toff) * WR, 3 * HR},  // 19
    {(2 + toff) * WR, 3 * HR},  // 20
    {(1 + toff) * WR, 3 * HR},  // 21
};
std::vector<std::array<uint32_t,3>> ico_face = {
    // Top
    {3,6,9},
    {18,9,2},
    {19,2,11},
    {20,11,7},
    {21,7,13},
    // Bottom
    {0,8,1},
    {14,4,8},
    {15,5,4},
    {16,10,5},
    {17,12,10},
    // Belt
    {1,8,6},
    {6,8,9},
    {9,8,4},
    {2,9,4},
    {2,4,5},
    {2,5,11},
    {5,10,11},
    {11,10,7},
    {12,7,10},
    {13,12,7},
};
//BasicMesh drawIco(glm::vec3 pos, glm::vec3 rot, float scale, int divisions, bool wireframe = true, bool normalise = false) {
BasicMesh drawIco(int divisions, bool normalise = false) {
    Mesh newMesh;
    if (divisions > 1) {
        for (int i = 0; i < ico_face.size(); i++) {
            newMesh = newMesh + Mesh::divideFace(ico_vert, ico_uvs, ico_face, i, divisions, true);
        }
    }
    else
    {
        newMesh.vertices  = ico_vert;
        newMesh.triangles = ico_face;
        newMesh.uvs       = ico_uvs;
    }
    
    if (normalise) {
        Mesh::normaliseVerts(newMesh.vertices);
        float bonus_scale = sqrt(pow(GR, 2) + 1);
        newMesh.setScale(bonus_scale);
    }
    /*
    glColor3f(1.f, 1.f, 1.f);
    newMesh.drawFilled();
    if (wireframe) {
        glColor3f(0.f, 0.f, 0.f);
        newMesh.drawWireFrame();
    }
    */
    return newMesh.getBasicMesh();
}
/*
void drawSphere(glm::vec3 pos, float scale, int divisions = 10, bool wireframe = false) {
    drawIco(pos, glm::vec3(0,0,0), scale, divisions, wireframe, true);
}
void drawIco(glm::vec3 pos, Quaternion rot, float scale, int divisions, bool wireframe = true, bool normalise = false) {
    rot = rot / Quaternion::abs(rot);
    EulerAngles angles = ToEulerAngles(rot);
    glm::vec3 rotation;
    rotation.x = angles.pitch;
    rotation.y = angles.roll;
    rotation.z = angles.yaw;
    drawIco(pos, rotation, scale, divisions, wireframe, normalise);
}
*/