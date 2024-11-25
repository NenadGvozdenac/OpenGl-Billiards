#ifndef COLORED_RECTANGLE
#define COLORED_RECTANGLE

#include "Rectangle.hpp"
#include "Image.hpp"

class ColoredRectangle : public Rectangle {
public:
	Image* image;

	ColoredRectangle(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int shaderProgram)
		: Rectangle(x1, y1, x2, y2, x3, y3, x4, y4, shaderProgram) {
	}

	void draw(const char* vsSource, const char* fsSource, const char* texturePath);
	void render();
};

#endif