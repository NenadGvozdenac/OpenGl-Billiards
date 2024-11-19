#ifndef BILLIARD_BALL_HPP
#define BILLIARD_BALL_HPP

#include "Constants.hpp"
#include "Pothole.hpp"
#include "TableEdge.hpp"

class BilliardBall : public Circle {
public:
	float vx, vy;
	float mass;
	
	BilliardBall(float x, float y, float radius);
	BilliardBall(float x, float y, float radius, float vx, float vy, float mass);

	void draw(const char* vsSource, const char* fsSource, const char* texturePath);
	void render();

	bool checkCollision(BilliardBall* ball);
	bool checkIfInHole(PotHole* hole);
	bool checkIfCollisionWithWall(TableEdge* edge);

	void applyImpulse(float impulseX, float impulseY);
	void applyFriction(float friction, float dt);

	void updatePosition(float dt);
};

#endif // BILLIARD_BALL_HPP
