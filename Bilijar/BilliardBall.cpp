#ifndef BILLIARD_BALL_CPP
#define BILLIARD_BALL_CPP

#include "BilliardBall.hpp"

BilliardBall::BilliardBall(float x, float y, float radius)
	: Circle(x, y, radius), vx(0), vy(0), mass(1) {}

BilliardBall::BilliardBall(float x, float y, float radius, float vx, float vy, float mass)
	: Circle(x, y, radius), vx(vx), vy(vy), mass(mass) {}

void BilliardBall::draw(const char* vsSource, const char* fsSource, const char* texturePath) {
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

	// Load the texture
	texture = new Image(texturePath);
	if (!texture->loadImage()) {
		cerr << "Failed to load image" << endl;
		return;
	}
}

void BilliardBall::render() {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

	glDrawArrays(GL_TRIANGLE_FAN, 0, Constants::NUM_SEGMENTS + 2);

	glBindVertexArray(0);
	glUseProgram(0);
}

bool BilliardBall::checkCollision(BilliardBall* ball) {
	return false;
}

bool BilliardBall::checkIfInHole(PotHole* hole) {
	return false;
}

bool BilliardBall::checkIfCollisionWithWall(TableEdge* edge) {
	return false;
}

void BilliardBall::applyImpulse(float impulseX, float impulseY) {
	vx += impulseX / mass;
	vy += impulseY / mass;
}

void BilliardBall::applyFriction(float friction, float dt) {
	float speed = sqrt(vx * vx + vy * vy);
	if (speed > 0) {
		float ratio = 1 - friction * dt;
		if (ratio < 0) ratio = 0;
		vx *= ratio;
		vy *= ratio;
	}
}

void BilliardBall::updatePosition(float dt) {
	x += vx * dt;
	y += vy * dt;
}

#endif