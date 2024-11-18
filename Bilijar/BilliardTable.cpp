#ifndef BILLIARD_TABLE_CPP
#define BILLIARD_TABLE_CPP

#include "Constants.hpp"
#include "Shader.hpp"
#include "BilliardTable.hpp"

// Indices for two triangles that form the quad
GLuint indices[] = {
    0, 1, 2,   // First triangle
    0, 2, 3    // Second triangle
};

void BilliardTable::drawTable(const char* vsSource, const char* fsSource, const char* texturePath) {
    // Compile and link shaders
    shaderProgram = createShader(vsSource, fsSource);

    // Vertex data
    float vertices[] = {
        -0.75f,  0.9f,  0.0f, 1.0f,   // Top-left
        -0.75f, -0.9f,  0.0f, 0.0f,   // Bottom-left
         0.75f, -0.9f,  1.0f, 0.0f,   // Bottom-right
         0.75f,  0.9f,  1.0f, 1.0f    // Top-right
    };

    unsigned int stride = (2 + 2) * sizeof(float);

    // Create VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Load texture
    texture = new Image(texturePath);
    if (!texture->loadImage()) {
        cerr << "Failed to load image" << endl;
        return;
    }
}

void BilliardTable::renderTable() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

#endif // !BILLIARD_TABLE_CPP
