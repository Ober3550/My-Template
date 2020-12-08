#pragma once
#include <vector>
#include <array>
#include <glm/vec3.hpp>
#include <SFML/OpenGL.hpp>
#include "Quaternion.h"
#include <glm\ext\vector_float2.hpp>
struct BasicMesh {
    std::vector<float> vert_uvs;
    std::vector<uint32_t> faces;
};
struct Mesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<std::array<uint32_t, 3>> triangles;
    glm::vec3 pos;
    glm::vec3 rotation;
    float scale;
    Mesh() {
        pos = {};
        rotation = {};
    }
    Mesh operator+(Mesh other) {
        int triOff = this->vertices.size();
        for (int i = 0; i < other.vertices.size(); i++) {
            this->vertices.push_back(other.vertices[i]);
        }
        for (int i = 0; i < other.uvs.size(); i++) {
            this->uvs.push_back(other.uvs[i]);
        }
        for (int i = 0; i < other.triangles.size(); i++) {
            std::array<uint32_t, 3> newTri = { other.triangles[i][0] + triOff, other.triangles[i][1] + triOff, other.triangles[i][2] + triOff };
            this->triangles.push_back(newTri);
        }
        return *this;
    }
    void drawWireFrame()
    {
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(rotation.x, 1, 0, 0);
        glRotatef(rotation.y, 0, 1, 0);
        glRotatef(rotation.z, 0, 0, 1);
        float scaleWire = scale * 1.01f;
        glScalef(scaleWire, scaleWire, scaleWire);
        glBegin(GL_LINES);
        for (int i = 0; i < triangles.size(); i++)
        {
            glVertex3f(vertices[triangles[i][0]].x, vertices[triangles[i][0]].y, vertices[triangles[i][0]].z);
            glVertex3f(vertices[triangles[i][1]].x, vertices[triangles[i][1]].y, vertices[triangles[i][1]].z);
            glVertex3f(vertices[triangles[i][1]].x, vertices[triangles[i][1]].y, vertices[triangles[i][1]].z);
            glVertex3f(vertices[triangles[i][2]].x, vertices[triangles[i][2]].y, vertices[triangles[i][2]].z);
            glVertex3f(vertices[triangles[i][2]].x, vertices[triangles[i][2]].y, vertices[triangles[i][2]].z);
            glVertex3f(vertices[triangles[i][0]].x, vertices[triangles[i][0]].y, vertices[triangles[i][0]].z);
        }
        glEnd();
        glScalef(1 / scaleWire, 1 / scaleWire, 1 / scaleWire);
        glRotatef(-rotation.x, 1, 0, 0);
        glRotatef(-rotation.y, 0, 1, 0);
        glRotatef(-rotation.z, 0, 0, 1);
        glTranslatef(-pos.x, -pos.y, -pos.z);
    }
    void drawFilled()
    {
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(rotation.x, 1, 0, 0);
        glRotatef(rotation.y, 0, 1, 0);
        glRotatef(rotation.z, 0, 0, 1);
        glScalef(scale, scale, scale);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < triangles.size(); i++) {
            for (int j = 0; j < 3; j++) {
                glVertex3f(vertices[triangles[i][j]][0], vertices[triangles[i][j]][1], vertices[triangles[i][j]][2]);
            }
        }
        glEnd();
        glScalef(1 / scale, 1 / scale, 1 / scale);
        glRotatef(-rotation.x, 1, 0, 0);
        glRotatef(-rotation.y, 0, 1, 0);
        glRotatef(-rotation.z, 0, 0, 1);
        glTranslatef(-pos.x, -pos.y, -pos.z);
    }
    BasicMesh getBasicMesh() {
        BasicMesh new_mesh;
        for (int i = 0; i < this->vertices.size(); i++) {
            new_mesh.vert_uvs.push_back(this->vertices[i].x);
            new_mesh.vert_uvs.push_back(this->vertices[i].y);
            new_mesh.vert_uvs.push_back(this->vertices[i].z);
            new_mesh.vert_uvs.push_back(this->uvs[i].x);
            new_mesh.vert_uvs.push_back(this->uvs[i].y);
        }
        for (int i = 0; i < this->triangles.size(); i++) {
            new_mesh.faces.push_back(this->triangles[i][0]);
            new_mesh.faces.push_back(this->triangles[i][1]);
            new_mesh.faces.push_back(this->triangles[i][2]);
        }
        return new_mesh;
    }
    void setScale(float s) {
        for (int i = 0; i < this->vertices.size(); i++) {
            vertices[i] = vertices[i] * s;
        }
    }
    static Mesh divideFace(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<std::array<uint32_t, 3>>& triangles, int idx, int div, bool accountProjection = false) {
        std::vector<glm::vec3>                  currentVerts;
        std::vector<glm::vec2>                  currentUvs;
        std::vector<glm::vec3>                  newVerts;
        std::vector<std::array<uint32_t, 3>>    newTris;
        std::vector<glm::vec2>                  newUvs;
        // Create original points for mapping new vertices
        currentVerts.push_back(vertices[triangles[idx][0]]);
        currentVerts.push_back(vertices[triangles[idx][1]]);
        currentVerts.push_back(vertices[triangles[idx][2]]);
        // Find edges that project vectors as 0 being origin
        glm::vec3 edgeA = currentVerts[1] - currentVerts[0];
        glm::vec3 edgeB = currentVerts[2] - currentVerts[0];
        // Do the same for Uvs
        currentUvs.push_back(uvs[triangles[idx][0]]);
        currentUvs.push_back(uvs[triangles[idx][1]]);
        currentUvs.push_back(uvs[triangles[idx][2]]);
        glm::vec2 uvEdgeA = currentUvs[1] - currentUvs[0];
        glm::vec2 uvEdgeB = currentUvs[2] - currentUvs[0];
        // Setup Indexes for faces
        int prevRow = 0;
        int row = 0;
        for (uint32_t i = 0; i < (div + 1); i++) {
            // Find leftmost and rightmost points in slice of triangle
            glm::vec3 leftVert = currentVerts[0] + float(i) * (edgeA / float(div));
            glm::vec3 rightVert = currentVerts[0] + float(i) * (edgeB / float(div));
            glm::vec3 horizontalEdge = rightVert - leftVert;
            // Do the same for Uvs
            glm::vec2 leftUv = currentUvs[0] + float(i) * (uvEdgeA / float(div));
            glm::vec2 rightUv = currentUvs[0] + float(i) * (uvEdgeB / float(div));
            glm::vec2 horitzontalUvEdge = rightUv - leftUv;
            // Adjust indexes for faces
            prevRow = row;
            row = newVerts.size();
            for (uint32_t j = 0; j < i; j++) {
                // Push new vertex
                newVerts.push_back(leftVert + float(j) * (horizontalEdge / float(i)));
                newUvs.push_back(leftUv + float(j) * (horitzontalUvEdge / float(i)));
                // Add triangle faces
                newTris.push_back({ row + j,row + 1 + j,prevRow + j });
                // Don't add a triangle face between last of this row and beginning of next row
                if (j != i - 1)
                    newTris.push_back({ prevRow + j, row + j + 1, prevRow + j + 1 });
            }
            newUvs.push_back(rightUv);
            newVerts.push_back(rightVert);
        }
        int triOffset = vertices.size();
        Mesh newMesh;
        newMesh.triangles = newTris;
        newMesh.vertices = newVerts;
        newMesh.uvs = newUvs;
        return newMesh;
    }
    static void normaliseVerts(std::vector<glm::vec3>& vertices) {
        for (int i = 0; i < vertices.size(); i++) {
            float mag = sqrt(pow(vertices[i].x, 2) + pow(vertices[i].y, 2) + pow(vertices[i].z, 2));
            vertices[i] /= mag;
        }
    }
};