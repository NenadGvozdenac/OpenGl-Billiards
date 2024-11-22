#ifndef BILLIARD_BALL_CPP
#define BILLIARD_BALL_CPP

#include "BilliardBall.hpp"

BilliardBall::BilliardBall(float x, float y, float radius, Enums::BilliardBallType ballType, float color[], int number)
	: Circle(x, y, radius), type(ballType), number(number), mass(1), vx(0), vy(0) {
	this->color = Color(color);
	this->potted = false;
}

BilliardBall::BilliardBall(const BilliardBall& ball) : Circle(ball.x, ball.y, ball.radius), type(ball.type), number(ball.number), mass(ball.mass), vx(ball.vx), vy(ball.vy) {
	this->color = Color(ball.color.r, ball.color.g, ball.color.b);
	this->potted = ball.potted;
}

BilliardBall::BilliardBall(float x, float y, float radius, float vx, float vy, float mass, Enums::BilliardBallType ballType, int number)
	: Circle(x, y, radius), vx(vx), vy(vy), mass(mass), type(ballType), number(number), potted(false) {}

void BilliardBall::draw(const char* vsSource, const char* fsSource, const char* texturePath) {
	shaderProgram = Shader::createShader(vsSource, fsSource);

	// Set up color uniform
	GLint colorUniformLocation = glGetUniformLocation(shaderProgram, "ballColor");
	glUseProgram(shaderProgram);

	// Pass the color values to the shader
	glUniform3f(colorUniformLocation, color.r, color.g, color.b);

	glUseProgram(0); // Unbind the shader program when done

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
	if (texturePath == nullptr) {
		cerr << "Texture path is null, skipping the loading." << endl;
		return;
	}

	texture = new Image(texturePath);
	if (!texture->loadImage()) {
		cerr << "Failed to load image" << endl;
		return;
	}
}

void BilliardBall::render(float dt) {
	// Check if the ball is potted
	if (potted) {
		return;
	}

	// Update position if the ball is moving
	if (moving()) {
		updatePosition(dt);
		applyFriction(Constants::FRICTION, dt);
	}

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	// Reapply the color uniform
	GLint colorUniformLocation = glGetUniformLocation(shaderProgram, "ballColor");
	glUniform3f(colorUniformLocation, color.r, color.g, color.b);

	// Bind texture if applicable
	if (texture != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}

	// Render the ball
	glDrawArrays(GL_TRIANGLE_FAN, 0, Constants::NUM_SEGMENTS + 2);

	glBindVertexArray(0);
	glUseProgram(0);
}

const bool BilliardBall::moving() const {
	return vx != 0 || vy != 0;
}

void BilliardBall::updateBuffer() {
	// Translate the circle vertices to the new position
	for (int i = 0; i < (Constants::NUM_SEGMENTS + 2) * 2; i += 2) {
		circle_buf[i] = radius * cos((Constants::PI / 180) * ((float)i / 2 * 360 / (float)Constants::NUM_SEGMENTS)) + x;
		circle_buf[i + 1] = radius * sin((Constants::PI / 180) * ((float)i / 2 * 360 / (float)Constants::NUM_SEGMENTS)) + y;
	}

	// Update the GPU buffer with the new data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(circle_buf), circle_buf);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool BilliardBall::checkCollision(BilliardBall* otherBall) {
	// Calculate the distance between the centers of the two balls
	float dx = otherBall->x - this->x;
	float dy = otherBall->y - this->y;
	float distance = sqrt(dx * dx + dy * dy);

	// Check if the balls are overlapping
	if (distance < this->radius + otherBall->radius) {
		// Calculate the unit normal and tangent vectors
		float nx = dx / distance;
		float ny = dy / distance;
		float tx = -ny;
		float ty = nx;

		// Project velocities onto the normal and tangent directions
		float v1n = vx * nx + vy * ny; // Normal velocity of this ball
		float v1t = vx * tx + vy * ty; // Tangential velocity of this ball
		float v2n = otherBall->vx * nx + otherBall->vy * ny; // Normal velocity of other ball
		float v2t = otherBall->vx * tx + otherBall->vy * ty; // Tangential velocity of other ball

		// Tangential velocities remain unchanged after collision
		float v1tFinal = v1t;
		float v2tFinal = v2t;

		// Compute the final normal velocities after collision using 1D elastic collision equations
		float v1nFinal = (v1n * (mass - otherBall->mass) + 2 * otherBall->mass * v2n) / (mass + otherBall->mass);
		float v2nFinal = (v2n * (otherBall->mass - mass) + 2 * mass * v1n) / (mass + otherBall->mass);

		// Convert the scalar normal and tangential velocities back into vectors
		vx = v1nFinal * nx + v1tFinal * tx;
		vy = v1nFinal * ny + v1tFinal * ty;
		otherBall->vx = v2nFinal * nx + v2tFinal * tx;
		otherBall->vy = v2nFinal * ny + v2tFinal * ty;

		// Resolve the overlap to prevent the balls from sticking
		float overlap = 0.5f * (this->radius + otherBall->radius - distance);
		this->x -= overlap * nx;
		this->y -= overlap * ny;
		otherBall->x += overlap * nx;
		otherBall->y += overlap * ny;

		return true; // Collision occurred
	}

	return false; // No collision
}

void calculateBallInHole(BilliardBall* ball) {
	switch (ball->type) {
		case Enums::BilliardBallType::CUE:
			ball->x = Constants::CUE_BALL_STARTING_POS_X;
			ball->y = Constants::CUE_BALL_STARTING_POS_Y;
			ball->vx = 0;
			ball->vy = 0;
			break;
		default:
			ball->potted = true;
	}
}

bool BilliardBall::checkIfInHole(PotHole* hole) {
	float x = hole->x;
	float y = hole->y;
	float radius = hole->radius;

	// Add a buffer to the hole radius
	float hitboxBuffer = 0.04f; // Adjust this value to increase the hitbox size
	float effectiveRadius = radius + hitboxBuffer;

	// Check if the ball is within the effective hitbox
	if (sqrt(pow(this->x - x, 2) + pow(this->y - y, 2)) <= effectiveRadius) {
		calculateBallInHole(this);
		updateBuffer();
		return true;
	}

	return false;
}

bool BilliardBall::checkIfCollisionWithWall(TableEdge* edge) {
	float x1 = edge->x1;
	float y1 = edge->y1;
	float x2 = edge->x2;
	float y2 = edge->y2;
	float x3 = edge->x3;
	float y3 = edge->y3;
	float x4 = edge->x4;
	float y4 = edge->y4;

	float offset = 0.02;

	switch (edge->type) {
	case TableEdgeType::TOP:
		if (y + radius <= y2 + offset && y + radius >= y1 - offset && x <= x4 && x >= x1) {
			vy = -vy;
			return true;
		}
		break;
	case TableEdgeType::BOTTOM:
		if (y - radius <= y1 + offset && y - radius >= y2 - offset && x <= x4 && x >= x1) {
			vy = -vy;
			return true;
		}
		break;
	case TableEdgeType::RIGHT:
		if (x + radius <= x1 + offset && x + radius >= x3 - offset && y <= y2 && y >= y1) {
			vx = -vx;
			return true;
		}
		break;
	case TableEdgeType::LEFT:
		if (x - radius <= x3 + offset && x - radius >= x1 - offset && y <= y3 && y >= y4) {
			vx = -vx;
			return true;
		}
		break;
	}
}

void BilliardBall::applyImpulse(float impulseX, float impulseY) {
	vx += impulseX / mass;
	vy += impulseY / mass;
}

void BilliardBall::applyFriction(float friction, float dt) {
	float speed = sqrt(vx * vx + vy * vy);
	if (speed > 0.01f) {
		float ratio = 1 - friction * dt;
		if (ratio < 0) ratio = 0;
		vx *= ratio;
		vy *= ratio;
	}
	else {
		vx = 0;
		vy = 0;
	}
}

void BilliardBall::updatePosition(float dt) {
	x += vx * dt;
	y += vy * dt;

	updateBuffer();
}

void BilliardBall::hitBall(float angle, float speed) {
	// Convert angle to radians if it is in degrees
	float angleInRadians = (angle + 180.f) * (Constants::PI / 180.0f);

	// Set velocity components based on angle and speed
	vx = speed * cos(angleInRadians);
	vy = speed * sin(angleInRadians);
}

void BilliardBall::reset() {
	x = Constants::CUE_BALL_STARTING_POS_X;
	y = Constants::CUE_BALL_STARTING_POS_Y;
	vx = 0;
	vy = 0;
	updateBuffer();
}

void BilliardBall::move(Enums::MOVE_DIRECTION move_direction, float move_quantity) {
	switch (move_direction) {
	case Enums::MOVE_DIRECTION::UP:
		y += move_quantity;
		break;
	case Enums::MOVE_DIRECTION::DOWN:
		y -= move_quantity;
		break;
	case Enums::MOVE_DIRECTION::LEFT:
		x -= move_quantity;
		break;
	case Enums::MOVE_DIRECTION::RIGHT:
		x += move_quantity;
		break;
	}
	updateBuffer();
}

#endif