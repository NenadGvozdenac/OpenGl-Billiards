#ifndef CUE_HPP
#define CUE_HPP

#include "Rectangle.hpp"
#include "BilliardBall.hpp"

class Cue : public Rectangle {
public:
	BilliardBall* cueBall;
	bool visible;
	float angle;

	float hitSpeed;

	Color color;
	Image* image;

	Cue(BilliardBall* cueBall, float width, float height, float angle, bool visible, float color[]);

	void draw(const char* vsSource, const char* fsSource, const char* texturePath);
	void render();

	void rotateCue(float xpos, float ypos);

	void switchCueSpeed(Enums::HIT_SPEED);

	static const float DEFAULT_WIDTH;
	static const float DEFAULT_HEIGHT;
	static float DEFAULT_COLOR[];
};

#endif