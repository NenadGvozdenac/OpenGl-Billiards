#define _CRT_SECURE_NO_WARNINGS

#include <thread>
#include <chrono>

#include "Constants.hpp"

#include "BilliardTable.hpp"
#include "TableEdge.hpp"
#include "BilliardBall.hpp"
#include "Cue.hpp"

using namespace std;

const bool DISPLAY_EDGES = false;
const float TARGET_FPS = 144.0f;
const float TARGET_FRAME_TIME = 1.0f / TARGET_FPS;
bool GAME_STARTED = false;
const float TEST_MOVE_SPEED = 0.03f;

static bool resetCue(BilliardBall& cueBall, Cue& cue) {
	cue = Cue(&cueBall, 1.f, 0.025f, 180, true, Color::BROWN);
	cue.draw("basic.vert", "ball.frag", nullptr);
	return true;
}

static void moveCueBall(Cue& cue, BilliardBall& cueBall, Enums::MOVE_DIRECTION move_direction, float move_quantity) {
	cueBall.move(move_direction, move_quantity);
	resetCue(cueBall, cue);
}

int main() {
	if (!glfwInit()) {
		cerr << "Failed to initialize GLFW" << endl;
		return -1;
	}

	glfwWindowHint(GLFW_RESIZABLE, Constants::windowResizable);

	GLFWwindow* window = glfwCreateWindow(Constants::windowWidth, Constants::windowHeight, Constants::windowName, NULL, NULL);

	if (!window) {
		cerr << "Failed to create window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowPos(window, Constants::CalculateWindowPositionX(), Constants::CalculateWindowPositionY());
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		glfwTerminate();
		return -1;
	}

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

	// Create and configure the billiard table
	BilliardTable table;
	table.drawTable("basic.vert", "basic.frag", "strides/billiard_table.png");

	// Top Edges with smaller height (height ~ 0.01)
	TableEdge leftUpperEdge(-0.645f, 0.375f, -0.67f, 0.385f, -0.0325f, 0.385f, -0.0325f, 0.375f, TableEdgeType::TOP, DISPLAY_EDGES);
	leftUpperEdge.drawEdge("basic.vert", "edgefrag.frag");

	TableEdge rightUpperEdge(0.0465f, 0.375f, 0.0465f, 0.385f, 0.685f, 0.385f, 0.655f, 0.375f, TableEdgeType::TOP, DISPLAY_EDGES);
	rightUpperEdge.drawEdge("basic.vert", "edgefrag.frag");

	// Bottom Edges with smaller height (height ~ 0.01)
	TableEdge leftLowerEdge(-0.645f, -0.375f, -0.67f, -0.385f, -0.0325f, -0.385f, -0.0325f, -0.375f, TableEdgeType::BOTTOM, DISPLAY_EDGES);
	leftLowerEdge.drawEdge("basic.vert", "edgefrag.frag");

	TableEdge rightLowerEdge(0.0465f, -0.375f, 0.0465f, -0.385f, 0.685f, -0.385f, 0.655f, -0.375f, TableEdgeType::BOTTOM, DISPLAY_EDGES);
	rightLowerEdge.drawEdge("basic.vert", "edgefrag.frag");

	// Left Edges with smaller width (width ~ 0.01)
	TableEdge leftEdge(-0.715f, -0.343f, -0.715f, 0.35f, -0.705f, 0.34f, -0.705f, -0.333f, TableEdgeType::LEFT, DISPLAY_EDGES);
	leftEdge.drawEdge("basic.vert", "edgefrag.frag");

	// Right Edges with smaller width (width ~ 0.01)
	TableEdge rightEdge(0.723f, -0.343f, 0.723f, 0.35f, 0.713f, 0.34f, 0.713f, -0.333f, TableEdgeType::RIGHT, DISPLAY_EDGES);
	rightEdge.drawEdge("basic.vert", "edgefrag.frag");


	PotHole potHole1(0.725f, 0.39f, 0.04f);
	potHole1.draw("basic.vert", "circle.frag", nullptr);

	PotHole potHole2(-0.713f, 0.39f, 0.04f);
	potHole2.draw("basic.vert", "circle.frag", nullptr);

	PotHole potHole3(0.725f, -0.376f, 0.04f);
	potHole3.draw("basic.vert", "circle.frag", nullptr);

	PotHole potHole4(-0.715f, -0.378f, 0.04f);
	potHole4.draw("basic.vert", "circle.frag", nullptr);

	PotHole potHole5(0.005f, 0.42f, 0.04f);
	potHole5.draw("basic.vert", "circle.frag", nullptr);

	PotHole potHole6(0.005f, -0.405f, 0.04f);
	potHole6.draw("basic.vert", "circle.frag", nullptr);

	BilliardBall cueBall(Constants::CUE_BALL_STARTING_POS_X, Constants::CUE_BALL_STARTING_POS_Y, 0.03f, Enums::BilliardBallType::CUE, Color::WHITE, 0);
	cueBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall solidYellowBall(0.32f, 0.0f, 0.03f, Enums::BilliardBallType::SOLID, Color::YELLOW, 0);
	solidYellowBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall solidRedBall(0.37f, 0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::RED, 0);
	solidRedBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall stripedBlueBall(0.37f, -0.03f, 0.03f, Enums::BilliardBallType::STRIPE, Color::BLUE, 0);
	stripedBlueBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall stripedOrangeBall(0.42f, 0.06f, 0.03f, Enums::BilliardBallType::STRIPE, Color::ORANGE, 0);
	stripedOrangeBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall blackBall(0.42f, 0.0f, 0.03f, Enums::BilliardBallType::BLACK, Color::BLACK, 0);
	blackBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall solidGreenBall(0.42f, -0.06f, 0.03f, Enums::BilliardBallType::SOLID, Color::GREEN, 0);
	solidGreenBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall solidBlueBall(0.47f, 0.09f, 0.03f, Enums::BilliardBallType::SOLID, Color::BLUE, 0);
	solidBlueBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall stripedBrownBall(0.47f, 0.03f, 0.03f, Enums::BilliardBallType::STRIPE, Color::BROWN, 0);
	stripedBrownBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall solidPurpleBall(0.47f, -0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::PURPLE, 0);
	solidPurpleBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall stripedYellowBall(0.47f, -0.09f, 0.03f, Enums::BilliardBallType::STRIPE, Color::YELLOW, 0);
	stripedYellowBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall solidBrownBall(0.52f, 0.12f, 0.03f, Enums::BilliardBallType::SOLID, Color::BROWN, 0);
	solidBrownBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall stripedPurpleBall(0.52f, 0.06f, 0.03f, Enums::BilliardBallType::STRIPE, Color::PURPLE, 0);
	stripedPurpleBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall solidOrangeBall(0.52f, 0.0f, 0.03f, Enums::BilliardBallType::SOLID, Color::ORANGE, 0);
	solidOrangeBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall stripedRedBall(0.52f, -0.06f, 0.03f, Enums::BilliardBallType::STRIPE, Color::RED, 0);
	stripedRedBall.draw("basic.vert", "ball.frag", nullptr);

	BilliardBall stripedGreenBall(0.52f, -0.12f, 0.03f, Enums::BilliardBallType::STRIPE, Color::GREEN, 0);
	stripedGreenBall.draw("basic.vert", "ball.frag", nullptr);

	Cue cue(&cueBall, 1.f, 0.025f, 180, true, Color::BROWN);
	cue.draw("basic.vert", "ball.frag", nullptr);

	float previousTime = glfwGetTime();  // Initialize previous time
	float dt = 0.0f;  // Delta time

	bool RECREATED_CUE = false;

	while (!glfwWindowShouldClose(window)) {
		float frameStartTime = glfwGetTime();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// Get current time and compute delta time (dt)
		float currentTime = glfwGetTime();
		dt = currentTime - previousTime;  // Calculate the difference
		previousTime = currentTime;  // Update previousTime for the next iteration

		// Left click
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			cue.rotateCue(xpos, ypos);
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !cueBall.moving()) {
			float angle = cue.angle;
			cueBall.hitBall(angle, cue.hitSpeed);
			GAME_STARTED = true;
			RECREATED_CUE = false;
		}

		// On press q, reset the cue ball
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			cueBall.reset();
			resetCue(cueBall, cue);
		}

		// TODO: For testing purposes only. Remove this later
		// On press w, move the cue ball up
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::UP, TEST_MOVE_SPEED);

		// TODO: For testing purposes only. Remove this later
		// On press s, move the cue ball down
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::DOWN, TEST_MOVE_SPEED);

		// TODO: For testing purposes only. Remove this later
		// On press a, move the cue ball left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::LEFT, TEST_MOVE_SPEED);

		// TODO: For testing purposes only. Remove this later
		// On press d, move the cue ball right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::RIGHT, TEST_MOVE_SPEED);

		// 1 - Very slow speed; 2 - Slow speed; 3 - Normal speed; 4 - Fast speed; 5 - Very fast speed
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			cue.switchCueSpeed(Enums::HIT_SPEED::VERY_SLOW);
		} else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			cue.switchCueSpeed(Enums::HIT_SPEED::SLOW);
		} else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
			cue.switchCueSpeed(Enums::HIT_SPEED::MEDIUM);
		} else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
			cue.switchCueSpeed(Enums::HIT_SPEED::FAST);
		} else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
			cue.switchCueSpeed(Enums::HIT_SPEED::VERY_FAST);
		}

		if (cueBall.moving()) {
			cue.visible = false;

			// Check for collisions with the edges
			cueBall.checkIfCollisionWithWall(&leftUpperEdge);
			cueBall.checkIfCollisionWithWall(&rightUpperEdge);
			cueBall.checkIfCollisionWithWall(&leftLowerEdge);
			cueBall.checkIfCollisionWithWall(&rightLowerEdge);
			cueBall.checkIfCollisionWithWall(&rightEdge);
			cueBall.checkIfCollisionWithWall(&leftEdge);

			// Check for collisions with the holes
			cueBall.checkIfInHole(&potHole1);
			cueBall.checkIfInHole(&potHole2);
			cueBall.checkIfInHole(&potHole3);
			cueBall.checkIfInHole(&potHole4);
			cueBall.checkIfInHole(&potHole5);
			cueBall.checkIfInHole(&potHole6);

		}
		else if (GAME_STARTED) {
			cue.visible = true;

			if (!RECREATED_CUE) {
				RECREATED_CUE = resetCue(cueBall, cue);
			}
		}

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the table
		table.renderTable();

		// Render the edges
		leftUpperEdge.renderEdge();
		rightUpperEdge.renderEdge();
		leftLowerEdge.renderEdge();
		rightLowerEdge.renderEdge();
		leftEdge.renderEdge();
		rightEdge.renderEdge();

		// Render the pothole
		potHole1.render(dt);
		potHole2.render(dt);
		potHole3.render(dt);
		potHole4.render(dt);
		potHole5.render(dt);
		potHole6.render(dt);

		// Render the cue ball
		cueBall.render(dt);

		// Render the other balls
		blackBall.render(dt);
		solidYellowBall.render(dt);
		stripedOrangeBall.render(dt);
		solidOrangeBall.render(dt);
		solidRedBall.render(dt);
		stripedBlueBall.render(dt);
		solidGreenBall.render(dt);
		solidBlueBall.render(dt);
		stripedBrownBall.render(dt);
		solidPurpleBall.render(dt);
		stripedYellowBall.render(dt);
		solidBrownBall.render(dt);
		stripedPurpleBall.render(dt);
		solidOrangeBall.render(dt);
		stripedRedBall.render(dt);
		stripedGreenBall.render(dt);

		// Render the cue
		cue.render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		// Frame end time and FPS cap
		float frameEndTime = glfwGetTime();
		float frameDuration = frameEndTime - frameStartTime;

		if (frameDuration < TARGET_FRAME_TIME) {
			float sleepTime = TARGET_FRAME_TIME - frameDuration;
			std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
		}
	}

	glfwTerminate();
	return 0;
}