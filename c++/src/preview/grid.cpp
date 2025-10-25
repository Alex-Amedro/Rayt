#include "preview/grid.hpp"

Grid::Grid(float size, int divisions) 
    : divisions(divisions), VAO(0), VBO(0), EBO(0) {
    
    float half = size / 2.0f;
    float step = size / static_cast<float>(divisions);
    
    // Générer les lignes parallèles à l'axe X
    for (int i = 0; i <= divisions; ++i) {
        float z = -half + i * step;
        vertices.push_back(glm::vec3(-half, 0.0f, z));
        vertices.push_back(glm::vec3(half, 0.0f, z));
    }
    
    // Générer les lignes parallèles à l'axe Z
    for (int i = 0; i <= divisions; ++i) {
        float x = -half + i * step;
        vertices.push_back(glm::vec3(x, 0.0f, -half));
        vertices.push_back(glm::vec3(x, 0.0f, half));
    }
    
    // Configurer les buffers OpenGL
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Grid::~Grid() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Grid::draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.size()));
    glBindVertexArray(0);
}
