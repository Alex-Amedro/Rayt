#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

class Grid {
public:

    Grid(float size, int divisions);
    ~Grid();
    void draw();

private:

    int divisions;
    GLuint VAO, VBO, EBO;
    std::vector<glm::vec3> vertices;
};

