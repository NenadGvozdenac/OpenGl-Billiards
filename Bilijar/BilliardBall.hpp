#ifndef BILLIARD_BALL_HPP
#define BILLIARD_BALL_HPP

#include "Constants.hpp"
#include "Pothole.hpp"
#include "TableEdge.hpp"

class BilliardBall : public Circle {
public:
	float vx, vy;
	float mass;

	int number;
	Enums::BilliardBallType type;
	Color color;
	
	BilliardBall(float x, float y, float radius, Enums::BilliardBallType type, float color[], int number = 0);
	BilliardBall(float x, float y, float radius, float vx, float vy, float mass, Enums::BilliardBallType type, int number = 0);

	void draw(const char* vsSource, const char* fsSource, const char* texturePath);
	void render(float dt);
	void updateBuffer();

	const bool moving() const;

	void reset();

	void hitBall(float angle, float dt);

	bool checkCollision(BilliardBall* ball);
	bool checkIfInHole(PotHole* hole);
	bool checkIfCollisionWithWall(TableEdge* edge);

	void applyImpulse(float impulseX, float impulseY);
	void applyFriction(float friction, float dt);

	void updatePosition(float dt);

	void move(Enums::MOVE_DIRECTION move_direction, float move_quantity);
};

#endif // BILLIARD_BALL_HPP
