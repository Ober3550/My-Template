#pragma once
#include <vector>
#include <array>
#include <glm/vec3.hpp>
#include <SFML/OpenGL.hpp>

struct Mesh {
    std::vector<glm::vec3> vertices;
    std::vector<std::array<int, 3>> triangles;
    Mesh() {

    }
    Mesh operator+(Mesh other) {
        int triOff = this->vertices.size();
        for (int i = 0; i < other.vertices.size(); i++) {
            this->vertices.push_back(other.vertices[i]);
        }
        for (int i = 0; i < other.triangles.size(); i++) {
            std::array<int, 3> newTri = { other.triangles[i][0] + triOff, other.triangles[i][1] + triOff, other.triangles[i][2] + triOff };
            this->triangles.push_back(newTri);
        }
        return *this;
    }
    void drawWireFrame()
    {
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
    }
    void drawFilled()
    {
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < triangles.size(); i++)
        {
            for (int j = 0; j < 3; j++)
            {
                glVertex3f(vertices[triangles[i][j]][0], vertices[triangles[i][j]][1], vertices[triangles[i][j]][2]);
            }
        }
        glEnd();
    }
};