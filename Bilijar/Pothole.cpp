#ifndef POTHOLE_CPP
#define POTHOLE_CPP

#include "Pothole.hpp"
#include "Constants.hpp"
#include <vector>

PotHole::PotHole(float x, float y, float radius) : Circle(x, y, radius) {}

void PotHole::draw(const char* vsSource, const char* fsSource, const char* texturePath) {
    if (shaderProgram == 0)
        shaderProgram = Shader::createShader(vsSource, fsSource);

    // Generate the circle vertices
    for (int i = -1; i <= Constants::NUM_SEGMENTS; i++) {
        circle_buf[2 + 2 * i] = radius * cos((Constants::PI / 180) * ((float)i * 360 / (float)Constants::NUM_SEGMENTS));
        circle_buf[2 + 2 * i + 1] = radius * sin((Constants::PI / 180) * ((float)i * 360 / (float)Constants::NUM_SEGMENTS));
    }

    // Translate the circle to the correct position
    for (int i = 0; i < (Constants::NUM_SEGMENTS + 2) * 2; i += 2) {
        circle_buf[i] += x;
        circle_buf[i + 1] += y;
	}

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle_buf), circle_buf, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PotHole::render(float dt) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle_buf) / (2 * sizeof(float)));
    glUseProgram(0);
    glBindVertexArray(0);
}

#endif