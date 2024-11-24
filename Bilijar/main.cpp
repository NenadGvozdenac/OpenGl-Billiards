#define _CRT_SECURE_NO_WARNINGS

#include <thread>
#include <chrono>

#include "Constants.hpp"

#include "BilliardTable.hpp"
#include "TableEdge.hpp"
#include "BilliardBall.hpp"
#include "Cue.hpp"
#include "Character.hpp"

const bool DISPLAY_EDGES = false;
const float TARGET_FPS = 144.0f;
const float TARGET_FRAME_TIME = 1.0f / TARGET_FPS;
bool GAME_STARTED = false;
const float TEST_MOVE_SPEED = 0.03f;

bool FIRST_BALL_HIT = false;

bool GAME_OVER = false;

static bool resetCue(BilliardBall& cueBall, Cue& cue) {
	cue = Cue(&cueBall, 1.f, 0.025f, 180, true, Color::BROWN);
	cue.draw("basic.vert", "ball.frag", nullptr);
	return true;
}

static void moveCueBall(Cue& cue, BilliardBall& cueBall, Enums::MOVE_DIRECTION move_direction, float move_quantity) {
	cueBall.move(move_direction, move_quantity);
	resetCue(cueBall, cue);
}

static void resetAllBalls(vector<BilliardBall>& balls);

static void checkCollisionsWithBalls(BilliardBall& ball, std::vector<BilliardBall>& balls) {
	if (ball.potted) return;
	for (BilliardBall& otherBall : balls) {
		if (otherBall.potted) continue;
		if (ball.number != otherBall.number) {
			if (ball.checkCollision(&otherBall)) {
				// If the cue ball hits another ball, it's not a foul
				if (ball.type == Enums::BilliardBallType::CUE && !FIRST_BALL_HIT) {
					FIRST_BALL_HIT = true;

					if (ball.number == 0 && otherBall.number == 8) {
						cout << "Foul!" << endl;
						ball.reset();
						resetAllBalls(balls);
					}
				}
			}
		}
	}
}

static void checkCollisionsWithEdges(BilliardBall& ball, std::vector<TableEdge>& edges) {
	for (TableEdge& edge : edges) {
		ball.checkIfCollisionWithWall(&edge);
	}
}

static void checkCollisionsWithHoles(BilliardBall& ball, std::vector<PotHole>& holes) {
	for (PotHole& hole : holes) {
		ball.checkIfInHole(&hole);
	}
}

static void drawBilliardTable(BilliardTable& table) {
	table.drawTable("basic.vert", "basic.frag", "strides/billiard_table.png");
}

static void drawEdges(std::vector<TableEdge>& edges) {
	for (TableEdge& edge : edges) {
		edge.drawEdge("basic.vert", "edgefrag.frag");
	}
}

static void drawPotholes(std::vector<PotHole>& holes) {
	for (PotHole& hole : holes) {
		hole.draw("basic.vert", "circle.frag", nullptr);
	}
}

static void drawBalls(std::vector<BilliardBall>& balls) {
	for (BilliardBall& ball : balls) {
		ball.draw("basic.vert", "ball.frag", nullptr);
	}
}

static void drawCue(BilliardBall& cueBall, Cue& cue) {
	cue.draw("basic.vert", "ball.frag", nullptr);
}

static void renderBilliardTable(BilliardTable& table) {
	table.renderTable();
}

static void renderEdges(std::vector<TableEdge>& edges) {
	for (TableEdge& edge : edges) {
		edge.renderEdge();
	}
}

static void renderPotholes(std::vector<PotHole>& holes, float dt) {
	for (PotHole& hole : holes) {
		hole.render(dt);
	}
}

static void renderBalls(std::vector<BilliardBall>& balls, float dt) {
	for (BilliardBall& ball : balls) {
		ball.render(dt);
	}
}

static BilliardBall findBlackBall(std::vector<BilliardBall>& balls) {
	for (BilliardBall& ball : balls) {
		if (ball.type == Enums::BilliardBallType::BLACK) {
			return ball;
		}
	}
}

static bool isTurnOver(std::vector<BilliardBall>& balls) {
	bool isTurnOver = false;

	std::vector<std::pair<BilliardBall, bool>> movingBalls;

	for (BilliardBall& ball : balls) {
		movingBalls.push_back(std::make_pair(ball, ball.moving()));
	}

	for (std::pair<BilliardBall, bool> movingBall : movingBalls) {
		if (movingBall.second) {
			isTurnOver = false;
			break;
		}
		else {
			isTurnOver = true;
		}
	}

	return isTurnOver;
}

static bool checkIfFoul(std::vector<BilliardBall>& balls) {
	BilliardBall cueBall = balls[0];
	BilliardBall blackBall = findBlackBall(balls);

	if (cueBall.checkCollision(&blackBall)) {
		// If its the first ball hit, its a foul
		if (!FIRST_BALL_HIT) {
			return true;
		}
	}

	if (cueBall.potted) {
		return true;
	}

	return false;
}

static bool checkIfGameOver(std::vector<BilliardBall>& balls) {
	int solids = 0;
	int stripes = 0;

	BilliardBall blackBall = findBlackBall(balls);

	if (blackBall.potted) {
		return true;
	}

	for (BilliardBall& ball : balls) {
		if (ball.type == Enums::BilliardBallType::SOLID && ball.potted) {
			solids++;
		}
		else if (ball.type == Enums::BilliardBallType::STRIPE && ball.potted) {
			stripes++;
		}
	}

	if (solids == 7 || stripes == 7) {
		return true;
	}

	return false;
}

static void resetAllBalls(vector<BilliardBall>& balls) {
	balls = {
		BilliardBall(Constants::CUE_BALL_STARTING_POS_X, Constants::CUE_BALL_STARTING_POS_Y, 0.03f, Enums::BilliardBallType::CUE, Color::WHITE, 0), // Cue ball
		BilliardBall(0.32f, 0.0f, 0.03f, Enums::BilliardBallType::SOLID, Color::YELLOW, 1),    // Solid yellow
		BilliardBall(0.37f, 0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::BLUE, 2),     // Solid blue
		BilliardBall(0.37f, -0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::RED, 3),     // Solid red
		BilliardBall(0.42f, 0.06f, 0.03f, Enums::BilliardBallType::SOLID, Color::PURPLE, 4),   // Solid purple
		BilliardBall(0.42f, 0.0f, 0.03f, Enums::BilliardBallType::BLACK, Color::BLACK, 8),    // Solid orange
		BilliardBall(0.42f, -0.06f, 0.03f, Enums::BilliardBallType::SOLID, Color::GREEN, 6),   // Solid green
		BilliardBall(0.47f, 0.09f, 0.03f, Enums::BilliardBallType::SOLID, Color::BROWN, 7),    // Solid brown
		BilliardBall(0.47f, 0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::ORANGE, 5),    // Black ball
		BilliardBall(0.47f, -0.03f, 0.03f, Enums::BilliardBallType::STRIPE, Color::YELLOW, 9), // Striped yellow
		BilliardBall(0.47f, -0.09f, 0.03f, Enums::BilliardBallType::STRIPE, Color::BLUE, 10),  // Striped blue
		BilliardBall(0.52f, 0.12f, 0.03f, Enums::BilliardBallType::STRIPE, Color::RED, 11),    // Striped red
		BilliardBall(0.52f, 0.06f, 0.03f, Enums::BilliardBallType::STRIPE, Color::PURPLE, 12), // Striped purple
		BilliardBall(0.52f, 0.0f, 0.03f, Enums::BilliardBallType::STRIPE, Color::ORANGE, 13),  // Striped orange
		BilliardBall(0.52f, -0.06f, 0.03f, Enums::BilliardBallType::STRIPE, Color::GREEN, 14), // Striped green
		BilliardBall(0.52f, -0.12f, 0.03f, Enums::BilliardBallType::STRIPE, Color::BROWN, 15)  // Striped brown
	};

	drawBalls(balls);
}

static std::vector<Character> createCharacters(const std::string text, float startX, float startY, float charWidth, float charHeight) {
	std::vector<Character> characters;

	for (char c : text) {
		Character character(startX, startY, charWidth, charHeight);
		std::string finalPath;

		// If the character is a space, use the space character
		if (c == ' ') {
			finalPath = "space";
		}

		// If the character is a number, use the number character
		if (std::isdigit(c)) {
			finalPath = std::to_string(c - '0');
		}
		
		// If the character is a :, use doublecolon.png
		if (c == ':') {
			finalPath = "doublecolon";
		}

		// If the character is a /, use slash.png
		if (c == '/') {
			finalPath = "slash";
		}

		// If uppercase, add C_uppercase. If lowercase, add C_lowercase
		if (std::isalpha(c)) {
			if (std::isupper(c)) {
				finalPath = std::string(1, c) + "_uppercase";
			}
			else {
				finalPath = std::string(1, std::toupper(c)) + "_lowercase";
			}
		}

		std::string path = "characters/" + finalPath + ".png";
		const char* path1 = path.c_str();
		character.drawCharacter("basic.vert", "basic.frag", path1);
		characters.push_back(character);
		startX += charWidth;  // Move to the right for the next character
	}

	return characters;
}

static std::vector<Character> drawText(std::string text, float startX, float startY, float charWidth, float charHeight) {
	return createCharacters(text, startX, startY, charWidth, charHeight);
}

static void setCueSpeed(std::vector<Character>& cueSpeed, Enums::HIT_SPEED hitSpeed) {
	switch (hitSpeed) {
	case Enums::HIT_SPEED::VERY_SLOW:
		cueSpeed = drawText("Cue speed: 1", -0.9f, 0.91f, 0.03f, 0.05f);
		break;
	case Enums::HIT_SPEED::SLOW:
		cueSpeed = drawText("Cue speed: 2", -0.9f, 0.91f, 0.03f, 0.05f);
		break;
	case Enums::HIT_SPEED::MEDIUM:
		cueSpeed = drawText("Cue speed: 3", -0.9f, 0.91f, 0.03f, 0.05f);
		break;
	case Enums::HIT_SPEED::FAST:
		cueSpeed = drawText("Cue speed: 4", -0.9f, 0.91f, 0.03f, 0.05f);
		break;
	case Enums::HIT_SPEED::VERY_FAST:
		cueSpeed = drawText("Cue speed: 5", -0.9f, 0.91f, 0.03f, 0.05f);
		break;
	}
}

static void hideCueSpeed(std::vector<Character>& cueSpeed) {
	cueSpeed = drawText("", -0.9f, 0.91f, 0.03f, 0.05f);
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

	std::vector<TableEdge> edges = {
		TableEdge(-0.645f, 0.375f, -0.67f, 0.385f, -0.0325f, 0.385f, -0.0325f, 0.375f, TableEdgeType::TOP, DISPLAY_EDGES),
		TableEdge(0.0465f, 0.375f, 0.0465f, 0.385f, 0.685f, 0.385f, 0.655f, 0.375f, TableEdgeType::TOP, DISPLAY_EDGES),
		TableEdge(-0.645f, -0.375f, -0.67f, -0.385f, -0.0325f, -0.385f, -0.0325f, -0.375f, TableEdgeType::BOTTOM, DISPLAY_EDGES),
		TableEdge(0.0465f, -0.375f, 0.0465f, -0.385f, 0.685f, -0.385f, 0.655f, -0.375f, TableEdgeType::BOTTOM, DISPLAY_EDGES),
		TableEdge(-0.715f, -0.343f, -0.715f, 0.35f, -0.705f, 0.34f, -0.705f, -0.333f, TableEdgeType::LEFT, DISPLAY_EDGES),
		TableEdge(0.723f, -0.343f, 0.723f, 0.35f, 0.713f, 0.34f, 0.713f, -0.333f, TableEdgeType::RIGHT, DISPLAY_EDGES)
	};

	std::vector<PotHole> potholes = {
		PotHole(0.725f, 0.39f, 0.04f),
		PotHole(-0.713f, 0.39f, 0.04f),
		PotHole(0.725f, -0.376f, 0.04f),
		PotHole(-0.715f, -0.378f, 0.04f),
		PotHole(0.005f, 0.42f, 0.04f),
		PotHole(0.005f, -0.405f, 0.04f)
	};

	std::vector<BilliardBall> billiardBalls = {
		BilliardBall(Constants::CUE_BALL_STARTING_POS_X, Constants::CUE_BALL_STARTING_POS_Y, 0.03f, Enums::BilliardBallType::CUE, Color::WHITE, 0), // Cue ball
		BilliardBall(0.32f, 0.0f, 0.03f, Enums::BilliardBallType::SOLID, Color::YELLOW, 1),    // Solid yellow
		BilliardBall(0.37f, 0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::BLUE, 2),     // Solid blue
		BilliardBall(0.37f, -0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::RED, 3),     // Solid red
		BilliardBall(0.42f, 0.06f, 0.03f, Enums::BilliardBallType::SOLID, Color::PURPLE, 4),   // Solid purple
		BilliardBall(0.42f, 0.0f, 0.03f, Enums::BilliardBallType::BLACK, Color::BLACK, 8),    // Solid orange
		BilliardBall(0.42f, -0.06f, 0.03f, Enums::BilliardBallType::SOLID, Color::GREEN, 6),   // Solid green
		BilliardBall(0.47f, 0.09f, 0.03f, Enums::BilliardBallType::SOLID, Color::BROWN, 7),    // Solid brown
		BilliardBall(0.47f, 0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::ORANGE, 5),    // Black ball
		BilliardBall(0.47f, -0.03f, 0.03f, Enums::BilliardBallType::STRIPE, Color::YELLOW, 9), // Striped yellow
		BilliardBall(0.47f, -0.09f, 0.03f, Enums::BilliardBallType::STRIPE, Color::BLUE, 10),  // Striped blue
		BilliardBall(0.52f, 0.12f, 0.03f, Enums::BilliardBallType::STRIPE, Color::RED, 11),    // Striped red
		BilliardBall(0.52f, 0.06f, 0.03f, Enums::BilliardBallType::STRIPE, Color::PURPLE, 12), // Striped purple
		BilliardBall(0.52f, 0.0f, 0.03f, Enums::BilliardBallType::STRIPE, Color::ORANGE, 13),  // Striped orange
		BilliardBall(0.52f, -0.06f, 0.03f, Enums::BilliardBallType::STRIPE, Color::GREEN, 14), // Striped green
		BilliardBall(0.52f, -0.12f, 0.03f, Enums::BilliardBallType::STRIPE, Color::BROWN, 15)  // Striped brown
	};

	drawBilliardTable(table);
	drawEdges(edges);
	drawPotholes(potholes);
	drawBalls(billiardBalls);

	BilliardBall& cueBall = billiardBalls[0];
	Cue cue(&cueBall, 1.f, 0.025f, 180, true, Color::BROWN);
	cue.draw("basic.vert", "ball.frag", nullptr);

	float previousTime = glfwGetTime();  // Initialize previous time
	float dt = 0.0f;  // Delta time

	bool RECREATED_CUE = false;

	std::vector<Character> characters = drawText("RA133/2021", 0.5f, 0.91f, 0.03f, 0.05f);
	std::vector<Character> nameAndLastName = drawText("Nenad Gvozdenac", 0.5f, 0.85f, 0.03f, 0.05f);
	std::vector<Character> cueSpeed = drawText("Cue speed: 3", -0.9f, 0.91f, 0.03f, 0.05f);

	while (!glfwWindowShouldClose(window) && !GAME_OVER) {
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

		// Right click
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !cueBall.moving()) {
			float angle = cue.angle;
			cueBall.hitBall(angle, cue.hitSpeed);
			GAME_STARTED = true;
			RECREATED_CUE = false;
			FIRST_BALL_HIT = false;
			cue.visible = false;
			hideCueSpeed(cueSpeed);
		}

		if (checkIfGameOver(billiardBalls)) {
			GAME_OVER = true;
			cout << "Game over!" << endl;
		}

		// On press q, reset the cue ball
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			cueBall.reset();
			resetCue(cueBall, cue);
			resetAllBalls(billiardBalls);
			GAME_STARTED = false;
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::MEDIUM);
		}

		// Before game started, cue ball can be moved from y -0.3 to 0.3 
		if (!GAME_STARTED) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && cue.y1 <= 0.300f)
				moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::UP, TEST_MOVE_SPEED);

			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && cue.y2 >= -0.312f)
				moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::DOWN, TEST_MOVE_SPEED);

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && cue.x1 >= -0.57f)
				moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::LEFT, TEST_MOVE_SPEED);

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && cue.x3 <= -0.345f)
				moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::RIGHT, TEST_MOVE_SPEED);
		}

		// 1 - Very slow speed; 2 - Slow speed; 3 - Normal speed; 4 - Fast speed; 5 - Very fast speed
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::VERY_SLOW);
			cue.switchCueSpeed(Enums::HIT_SPEED::VERY_SLOW);
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::SLOW);
			cue.switchCueSpeed(Enums::HIT_SPEED::SLOW);
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::MEDIUM);
			cue.switchCueSpeed(Enums::HIT_SPEED::MEDIUM);
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::FAST);
			cue.switchCueSpeed(Enums::HIT_SPEED::FAST);
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::VERY_FAST);
			cue.switchCueSpeed(Enums::HIT_SPEED::VERY_FAST);
		}

		// Check if the cue ball is potted or black ball was first hit
		if (checkIfFoul(billiardBalls)) {
			cout << "Foul!" << endl;
			cueBall.reset();
			resetAllBalls(billiardBalls);
		}

		if (isTurnOver(billiardBalls) && GAME_STARTED) {
			if (!RECREATED_CUE) {
				resetCue(cueBall, cue);
				setCueSpeed(cueSpeed, Enums::HIT_SPEED::MEDIUM);
				RECREATED_CUE = true;
			}
		}

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the table
		renderBilliardTable(table);

		// Render the edges
		renderEdges(edges);

		// Render the pothole
		renderPotholes(potholes, dt);

		// Render the cue ball
		renderBalls(billiardBalls, dt);

		// Check all collision with balls
		for (BilliardBall& ball : billiardBalls) {
			checkCollisionsWithBalls(ball, billiardBalls);
		}

		// Check all collision with edges
		for (BilliardBall& ball : billiardBalls) {
			checkCollisionsWithEdges(ball, edges);
		}

		// Check all collision with holes
		for (BilliardBall& ball : billiardBalls) {
			checkCollisionsWithHoles(ball, potholes);
		}

		// Render the cue
		cue.render();

		// Render the index
		for (Character& character : characters) {
			character.renderCharacter();
		}

		// Render the name
		for (Character& character : nameAndLastName) {
			character.renderCharacter();
		}

		// Render the cue speed
		for (Character& character : cueSpeed) {
			character.renderCharacter();
		}

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