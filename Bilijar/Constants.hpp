#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Enums class
class Enums {
public:
	enum MOVE_DIRECTION {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	enum HIT_SPEED {
		VERY_SLOW,
		SLOW,
		MEDIUM,
		FAST,
		VERY_FAST
	};

	enum BALL_COLOR {
		WHITE_COLOR,
		BLUE_COLOR,
		ORANGE_COLOR,
		RED_COLOR,
		GREEN_COLOR,
		YELLOW_COLOR,
		BLACK_COLOR,
		PURPLE_COLOR,
		PINK_COLOR,
		BROWN_COLOR
	};
	
	enum BilliardBallType {
		CUE,
		STRIPE,
		SOLID,
		BLACK
	};
};

// Constants class
class Constants {
public:
	// Constants for the window
	static const char* windowName;
	static const int windowWidth;
	static const int windowHeight;
	static const int windowResizable;

	// Calculate the window position
	static int CalculateWindowPositionX();
	static int CalculateWindowPositionY();

	// Constants for the circles
	static const int NUM_SEGMENTS;
	static const float PI;

	// Constants for the physics
	static const float BALL_SPEED;
	static const float FRICTION;

	// Constants for the cue ball
	static const float CUE_BALL_STARTING_POS_X;
	static const float CUE_BALL_STARTING_POS_Y;

	static const unordered_map<Enums::HIT_SPEED, float> hitSpeedMap;

	static float getSpeed(Enums::HIT_SPEED hitSpeed);
};

// Shader class
class Shader {
public:
	static unsigned int compileShader(GLenum type, const char* source);
	static unsigned int createShader(const char* vsSource, const char* fsSource);
};

// Constants for the colors
class Color {
public:
	// Color values
	static float WHITE[];
	static float BLUE[];
	static float ORANGE[];
	static float RED[];
	static float GREEN[];
	static float YELLOW[];
	static float BLACK[];
	static float PURPLE[];
	static float PINK[];
	static float BROWN[];

	// Color map
	static std::unordered_map<Enums::BALL_COLOR, const float*> colorMap;

	// Constructor
	float r, g, b;
	Color() : r(0), g(0), b(0) {}
	Color(Color& color) : r(color.r), g(color.g), b(color.b) {}
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
	Color(float color[]) : r(color[0]), g(color[1]), b(color[2]) {}
	Color(Enums::BALL_COLOR color) : r(colorMap[color][0]), g(colorMap[color][1]), b(colorMap[color][2]) {}
};

#endif // CONSTANTS_HPP