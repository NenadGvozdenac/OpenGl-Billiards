#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

class Rectangle {
public:
	float x1, y1;
	float x2, y2;
	float x3, y3;
	float x4, y4;
	unsigned int shaderProgram;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	Rectangle(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float shaderProgram);
};

#endif