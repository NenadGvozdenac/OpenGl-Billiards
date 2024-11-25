#ifndef COLORED_RECTANGLE_CPP
#define COLORED_RECTANGLE_CPP

#include "ColoredRectangle.hpp"
#include "Constants.hpp"

void ColoredRectangle::draw(const char* vsSource, const char* fsSource, const char* texturePath) {
	if (shaderProgram == 0)
		shaderProgram = Shader::createShader(vsSource, fsSource);

	GLuint indices[] = {
		0, 1, 2,   // First triangle
		0, 2, 3    // Second triangle
	};

	float vertices[] = {
		x1, y1, 0.0f, 1.0f,   // Top-left
		x2, y2, 0.0f, 0.0f,   // Bottom-left
		x3, y3, 1.0f, 0.0f,   // Bottom-right
		x4, y4, 1.0f, 1.0f    // Top-right
	};

	unsigned int stride = (2 + 2) * sizeof(float);

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

	// Load and create a texture
	image = new Image(texturePath);
	image->loadImage();
}

void ColoredRectangle::render() {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, image->getTextureID());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

#endif