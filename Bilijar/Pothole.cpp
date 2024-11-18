#ifndef POTHOLE_CPP
#define POTHOLE_CPP

#include "Pothole.hpp"
#include "Constants.hpp"
#include <vector>

const int NUM_SEGMENTS = 30;
const float PI = 3.14159265359f;

float circle[(NUM_SEGMENTS + 2) * 2] = { 0 };

void PotHole::drawPotHole(const char* vsSource, const char* fsSource, const char* texturePath) {
    shaderProgram = Shader::createShader(vsSource, fsSource);

    // Generate the circle vertices
    for (int i = -1; i <= NUM_SEGMENTS; i++) {
        circle[2 + 2 * i] = radius * cos((PI / 180) * ((float)i * 360 / (float)NUM_SEGMENTS));
        circle[2 + 2 * i + 1] = radius * sin((PI / 180) * ((float)i * 360 / (float)NUM_SEGMENTS));
    }

    // Translate the circle to the correct position
    for (int i = 0; i < (NUM_SEGMENTS + 2) * 2; i += 2) {
		circle[i] += x;
		circle[i + 1] += y;
	}

    // Print the circle vertices
    for (int i = 0; i < (NUM_SEGMENTS + 2) * 2; i += 2) {
		cout << circle[i] << " " << circle[i + 1] << endl;
	}

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PotHole::renderPotHole() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));
    glUseProgram(0);
    glBindVertexArray(0);
}

#endif