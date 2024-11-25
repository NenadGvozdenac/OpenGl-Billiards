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
	this->hitSpeed = Constants::getSpeed(Enums::HIT_SPEED::MEDIUM);
}

void Cue::draw(const char* vsSource, const char* fsSource, const char* texturePath) {
	if (!visible) {
		return;
	}

	if (shaderProgram == 0)
		shaderProgram = Shader::createShader(vsSource, fsSource);

	GLuint indices[] = {
		0, 1, 2,   // First triangle
		0, 2, 3    // Second triangle
	};

	// Calculate rotated vertices
	float radians = angle * (Constants::PI / 180.0f);  // Convert angle to radians
	float cosAngle = cos(radians);
	float sinAngle = sin(radians);

	float vertices[] = {
		cueBall->x + (x1 - cueBall->x) * cosAngle - (y1 - cueBall->y) * sinAngle,
		cueBall->y + (x1 - cueBall->x) * sinAngle + (y1 - cueBall->y) * cosAngle,
		0.0f, 1.0f,

		cueBall->x + (x3 - cueBall->x) * cosAngle - (y3 - cueBall->y) * sinAngle,
		cueBall->y + (x3 - cueBall->x) * sinAngle + (y3 - cueBall->y) * cosAngle,
		0.0f, 0.0f,

		cueBall->x + (x4 - cueBall->x) * cosAngle - (y4 - cueBall->y) * sinAngle,
		cueBall->y + (x4 - cueBall->x) * sinAngle + (y4 - cueBall->y) * cosAngle,
		1.0f, 0.0f,

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

	// Bind image texture
	image = new Image(texturePath);
	image->loadImage();
}

void Cue::render() {
	if (!visible) {
		return;
	}

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, image->getTextureID());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Cue::rotateCue(float xpos, float ypos) {
	if (!visible) {
		return;
	}

	float windowWidth = Constants::windowWidth;
	float windowHeight = Constants::windowHeight;

	// Get the mouse position in normalized coordinates (0,0 to 1,1)
	float normalizedX = xpos / windowWidth;
	float normalizedY = ypos / windowHeight;

	// Convert normalized coordinates to actual window coordinates in the range [-1, 1]
	float normalizedXPos = (normalizedX * 2.0f) - 1.0f;  // Mapping to [-1, 1]
	float normalizedYPos = 1.0f - (normalizedY * 2.0f);  // Mapping to [-1, 1]

	// Translate normalized mouse position to actual coordinates in the window
	// Adjust to make the cue rotate around the cue ball's position
	float mouseX = normalizedXPos * windowWidth;
	float mouseY = normalizedYPos * windowHeight;

	// Now calculate the vector from the cue ball to the mouse cursor
	float dx = mouseX - cueBall->x;
	float dy = mouseY - cueBall->y;

	// Calculate the angle from the cue ball to the mouse cursor in degrees
	angle = atan2(dy, dx) * (180.0f / Constants::PI);

	// Redraw the cue with the new angle
	draw("basic.vert", "basic.frag", "strides/cue.png");
}

float calculateOffset(Enums::HIT_SPEED speed) {
	switch (speed) {
	case Enums::HIT_SPEED::VERY_SLOW:
		return 0.3f;
	case Enums::HIT_SPEED::SLOW:
		return 0.4f;
	case Enums::HIT_SPEED::MEDIUM:
		return 0.5f;
	case Enums::HIT_SPEED::FAST:
		return 0.6f;
	case Enums::HIT_SPEED::VERY_FAST:
		return 0.7f;
	}
}

void Cue::switchCueSpeed(Enums::HIT_SPEED speed) {
	hitSpeed = Constants::getSpeed(speed);
}

#endif