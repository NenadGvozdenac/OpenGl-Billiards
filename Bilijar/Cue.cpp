#ifndef CUE_CPP
#define CUE_CPP

#include "Cue.hpp"

const float Cue::DEFAULT_WIDTH = 0.5f;
const float Cue::DEFAULT_HEIGHT = 0.025f;

float Cue::DEFAULT_COLOR[] = { 0.647f, 0.165f, 0.165f };

const float HUE_VERTICAL_OFFSET = -0.012f;
const float HUE_HORIZONTAL_OFFSET = -0.045f;

const Color newColor = Color::BLUE;

Cue::Cue(BilliardBall* cueBall, float width, float height, float angle, bool visible, float color[])
	: Rectangle(cueBall->x, cueBall->y, cueBall->x + width, cueBall->y, cueBall->x, cueBall->y + height, cueBall->x + width, cueBall->y + height, 0) {
	this->x1 -= HUE_HORIZONTAL_OFFSET;
	this->x2 -= HUE_HORIZONTAL_OFFSET;
	this->x3 -= HUE_HORIZONTAL_OFFSET;
	this->x4 -= HUE_HORIZONTAL_OFFSET;

	this->y1 += HUE_VERTICAL_OFFSET;
	this->y2 += HUE_VERTICAL_OFFSET;
	this->y3 += HUE_VERTICAL_OFFSET;
	this->y4 += HUE_VERTICAL_OFFSET;
	
	this->cueBall = cueBall;
	this->angle = angle;
	this->visible = visible;
	this->color = Color(color);
}

void Cue::draw(const char* vsSource, const char* fsSource, const char* texturePath) {
	shaderProgram = Shader::createShader(vsSource, fsSource);

	// Set up color uniform
	GLint colorUniformLocation = glGetUniformLocation(shaderProgram, "ballColor");
	glUseProgram(shaderProgram);

	// Pass the color values to the shader
	glUniform3f(colorUniformLocation, color.r, color.g, color.b);

	glUseProgram(0); // Unbind the shader program when done

	GLuint indices[] = {
		0, 1, 2,   // First triangle
		0, 2, 3    // Second triangle
	};

	// Calculate rotated vertices
	float radians = angle * (Constants::PI / 180.0f);  // Convert angle to radians
	float cosAngle = cos(radians);
	float sinAngle = sin(radians);

	float vertices[] = {
		// Top-left
		cueBall->x + (x1 - cueBall->x) * cosAngle - (y1 - cueBall->y) * sinAngle,
		cueBall->y + (x1 - cueBall->x) * sinAngle + (y1 - cueBall->y) * cosAngle,
		0.0f, 1.0f,

		// Bottom-left
		cueBall->x + (x3 - cueBall->x) * cosAngle - (y3 - cueBall->y) * sinAngle,
		cueBall->y + (x3 - cueBall->x) * sinAngle + (y3 - cueBall->y) * cosAngle,
		0.0f, 0.0f,

		// Bottom-right
		cueBall->x + (x4 - cueBall->x) * cosAngle - (y4 - cueBall->y) * sinAngle,
		cueBall->y + (x4 - cueBall->x) * sinAngle + (y4 - cueBall->y) * cosAngle,
		1.0f, 0.0f,

		// Top-right
		cueBall->x + (x2 - cueBall->x) * cosAngle - (y2 - cueBall->y) * sinAngle,
		cueBall->y + (x2 - cueBall->x) * sinAngle + (y2 - cueBall->y) * cosAngle,
		1.0f, 1.0f
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
}

void Cue::render() {
	if (!visible) {
		return;
	}

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	// Re-apply color uniform in case the color changes dynamically
	GLint colorUniformLocation = glGetUniformLocation(shaderProgram, "ballColor");
	glUniform3f(colorUniformLocation, color.r, color.g, color.b);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

#endif