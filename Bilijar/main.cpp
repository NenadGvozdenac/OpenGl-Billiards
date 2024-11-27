#define _CRT_SECURE_NO_WARNINGS

#include <thread>
#include <chrono>

#include "Constants.hpp"

#include "BilliardTable.hpp"
#include "TableEdge.hpp"
#include "BilliardBall.hpp"
#include "Cue.hpp"
#include "Character.hpp"
#include "ColoredRectangle.hpp"

const bool DISPLAY_EDGES = false;
const float TARGET_FPS = 60.0f;
const float TARGET_FRAME_TIME = 1.0f / TARGET_FPS;
bool GAME_STARTED = false;
const float TEST_MOVE_SPEED = 0.03f;

bool FIRST_BALL_HIT = false;
bool GAME_OVER = false;
bool FIRST_TURN_OVER = false;

bool pauseOverlayVisible = false;
bool gameOverOverlayVisible = false;

bool FOUL = false;
bool STOP_WRITING = false;

int nextBallNumber = 1;
int turnNumberN = 1;
int ballsPottedNumber = 0;
int ballsLeftNumber = 9;

bool CUEBALL_VISIBLE = true;

typedef enum Player {
	PLAYER1,
	PLAYER2
} Player;

Player currentPlayer = PLAYER1;

std::vector<Character> drawText(std::string text, float startX, float startY, float charWidth, float charHeight);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, int button, int action, int mods);

void drawBilliardTable(BilliardTable& table);
void drawEdges(std::vector<TableEdge>& edges);
void drawPotholes(std::vector<PotHole>& potholes);
void drawBalls(std::vector<BilliardBall>& billiardBalls);
void drawCue(BilliardBall& cueBall, Cue& cue);
void drawRectangles(std::vector<ColoredRectangle>& rectangles);

void renderBilliardTable(BilliardTable& table);
void renderEdges(std::vector<TableEdge>& edges);
void renderPotholes(std::vector<PotHole>& holes, float dt);
void renderBalls(std::vector<BilliardBall>& balls, float dt);

void checkCollisionsWithBalls(BilliardBall& ball, std::vector<BilliardBall>& balls);
void checkCollisionsWithEdges(BilliardBall& ball, std::vector<TableEdge>& edges);
void checkCollisionsWithHoles(BilliardBall& ball, std::vector<PotHole>& holes);

void moveCueBall(Cue& cue, BilliardBall& cueBall, Enums::MOVE_DIRECTION move_direction, float move_quantity);

bool resetCue(BilliardBall& cueBall, Cue& cue);
bool isTurnOver(std::vector<BilliardBall>& balls);
bool checkIfFoul(std::vector<BilliardBall>& balls);
bool isEndOfTurn(std::vector<BilliardBall>& balls);
void resetAllBalls(std::vector<BilliardBall>& balls);
bool isGameOver(std::vector<BilliardBall>& balls);

BilliardBall findBlackBall(std::vector<BilliardBall>& balls);
BilliardBall* findNextBall(std::vector<BilliardBall>& balls);
void drawNextBall(BilliardBall** nextBall, Color color);

void setCueSpeed(std::vector<Character>& cueSpeed, Enums::HIT_SPEED hitSpeed);
void hideCueSpeed(std::vector<Character>& cueSpeed);

void renderOverlay(GLFWwindow* window, bool condition, std::vector<ColoredRectangle>& rectangles, std::vector<Character>& overlay, std::vector<Character>& continueText, std::vector<Character>& exitText);
void renderNextBall(BilliardBall* nextBall);

int findNumberOfBallsPotted(std::vector<BilliardBall>& balls);
int findNumberOfBallsLeft(std::vector<BilliardBall>& balls);

void setBallsPotted(std::vector<Character>& ballsPotted, int ballsPottedNumber);
void setBallsLeft(std::vector<Character>& ballsLeft, int ballsLeftNumber);
void setTurnNumber(std::vector<Character>& turnNumber, int turnNumberN);
void setCurrentPlayer(std::vector<Character>& currentPlayerText, Player player);
void setFoulText(std::vector<Character>& foulText, bool foul);

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

	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);

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
		BilliardBall(Constants::CUE_BALL_STARTING_POS_X, Constants::CUE_BALL_STARTING_POS_Y, 0.03f, Enums::BilliardBallType::CUE, Color::WHITE, 0),
		BilliardBall(0.32f, 0.0f, 0.03f, Enums::BilliardBallType::SOLID, Color::YELLOW, 1),
		BilliardBall(0.37f, 0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::BLUE, 2),
		BilliardBall(0.37f, -0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::RED, 3),
		BilliardBall(0.42f, 0.06f, 0.03f, Enums::BilliardBallType::SOLID, Color::PURPLE, 4),
		BilliardBall(0.42f, 0.0f, 0.03f, Enums::BilliardBallType::STRIPE, Color::PINK, 9),
		BilliardBall(0.42f, -0.06f, 0.03f, Enums::BilliardBallType::SOLID, Color::ORANGE, 5),
		BilliardBall(0.47f, 0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::GREEN, 6),
		BilliardBall(0.47f, -0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::BROWN, 7),
		BilliardBall(0.52f, 0.0f, 0.03f, Enums::BilliardBallType::BLACK, Color::BLACK, 8),
	};

	BilliardBall* nextBall = new BilliardBall(-0.55f, 0.871f, 0.03f, Enums::BilliardBallType::SOLID, Color::YELLOW, 1);
	nextBall->draw("basic.vert", "ball.frag", nullptr);

	std::vector<ColoredRectangle> coloredRectangles = {
		ColoredRectangle(-1.f, -1.f, 1.f, -1.f, 1.f, 1.f, -1.f, 1.f, 0.0f),
		ColoredRectangle(-0.2f, -0.1f, 0.2f, -0.1f, 0.2f, 0.1f, -0.2f, 0.1f, 0.0f),
		ColoredRectangle(-0.2f, -0.4f, 0.2f, -0.4f, 0.2f, -0.2f, -0.2f, -0.2f, 0.0f)
	};

	drawBilliardTable(table);
	drawEdges(edges);
	drawPotholes(potholes);
	drawBalls(billiardBalls);
	drawRectangles(coloredRectangles);

	std::vector<Character> index = drawText("RA133/2021", 0.5f, 0.91f, 0.03f, 0.05f);
	std::vector<Character> nameAndLastName = drawText("Nenad Gvozdenac", 0.5f, 0.85f, 0.03f, 0.05f);
	std::vector<Character> cueSpeed = drawText("Cue speed: 3", -0.9f, 0.91f, 0.03f, 0.05f);
	std::vector<Character> nextBallText = drawText("Next ball: ", -0.9f, 0.85f, 0.03f, 0.05f);
	std::vector<Character> ballsPotted = drawText("Balls potted: " + to_string(ballsPottedNumber), -0.9f, 0.79f, 0.03f, 0.05f);
	std::vector<Character> ballsLeft = drawText("Balls left: " + to_string(ballsLeftNumber), -0.9f, 0.73f, 0.03f, 0.05f);
	std::vector<Character> turnNumber = drawText("Turn number: " + to_string(turnNumberN), -0.9f, 0.67f, 0.03f, 0.05f);
	std::vector<Character> currentPlayerText = drawText("Current player: " + (currentPlayer == Player::PLAYER1 ? std::string("PLAYER1") : std::string("PLAYER2")) , -0.9f, 0.61f, 0.03f, 0.05f);
	std::vector<Character> foulText = drawText("", -0.1f, 0.55f, 0.05f, 0.1f);

	std::vector<Character> gamePaused = drawText("Game paused", -0.335f, 0.5f, 0.06f, 0.1f);
	std::vector<Character> continueText = drawText("Continue", -0.125f, -0.025f, 0.03f, 0.05f);
	std::vector<Character> exitText = drawText("Exit", -0.065f, -0.325f, 0.03f, 0.05f);

	std::vector<Character> gameOver = drawText("Game over", -0.3f, 0.5f, 0.06f, 0.1f);
	std::vector<Character> playAgainText = drawText("Play again", -0.155f, -0.025f, 0.03f, 0.05f);

	BilliardBall& cueBall = billiardBalls[0];
	Cue cue(&cueBall, 1.f, 0.025f, 180, true, Color::BROWN);
	cue.draw("basic.vert", "basic.frag", "strides/cue.png");

	float previousTime = glfwGetTime();  // Initialize previous time
	float dt = 0.0f;  // Delta time

	bool RECREATED_CUE = true;

	while (!glfwWindowShouldClose(window) && !GAME_OVER) {
		float frameStartTime = glfwGetTime();

		// Get current time and compute delta time (dt)
		float currentTime = glfwGetTime();
		dt = currentTime - previousTime;  // Calculate the difference
		previousTime = currentTime;  // Update previousTime for the next iteration

		// Left click
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !pauseOverlayVisible) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			cue.rotateCue(xpos, ypos);
		}

		// Right click
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !cueBall.moving() && !pauseOverlayVisible && RECREATED_CUE) {
			float angle = cue.angle;
			cueBall.hitBall(angle, cue.hitSpeed);
			GAME_STARTED = true;
			RECREATED_CUE = false;
			FIRST_BALL_HIT = false;
			cue.visible = false;
			hideCueSpeed(cueSpeed);

			setFoulText(foulText, false);
		}

		if (GAME_STARTED) {
			// On the end of turn
			if (isTurnOver(billiardBalls) && !pauseOverlayVisible) {
				FIRST_TURN_OVER = true;
			}
		}

		// On press q, reset the cue ball
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !pauseOverlayVisible) {
			cueBall.reset();
			resetCue(cueBall, cue);
			resetAllBalls(billiardBalls);
			GAME_STARTED = false;
			FIRST_BALL_HIT = false;
			FIRST_TURN_OVER = false;
			RECREATED_CUE = true;
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::MEDIUM);
			nextBallNumber = 1;

			turnNumberN = 1;
			ballsPottedNumber = 0;
			ballsLeftNumber = 9;

			setBallsPotted(ballsPotted, ballsPottedNumber);
			setBallsLeft(ballsLeft, ballsLeftNumber);
			setTurnNumber(turnNumber, turnNumberN);
			setCurrentPlayer(currentPlayerText, Player::PLAYER1);

			BilliardBall* ballNew = findNextBall(billiardBalls);

			if (ballNew != nullptr) {
				drawNextBall(&nextBall, ballNew->color);
			}
		}

		// On press e, reset the angle of the cue
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !pauseOverlayVisible) {
			cue.angle = 180;
			resetCue(cueBall, cue);
		}

		// Before game started, cue ball can be moved from y -0.312 to 0.3 ; x -0.57 to -0.345
		if (!GAME_STARTED && !pauseOverlayVisible) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && cue.y1 <= 0.300f)
				moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::UP, TEST_MOVE_SPEED);

			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && cue.y2 >= -0.312f)
				moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::DOWN, TEST_MOVE_SPEED);

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && cue.x1 >= -0.57f)
				moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::LEFT, TEST_MOVE_SPEED);

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && cue.x3 <= -0.345f)
				moveCueBall(cue, cueBall, Enums::MOVE_DIRECTION::RIGHT, TEST_MOVE_SPEED);
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && gameOverOverlayVisible) {
			// Get mouse position
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			int windowWidth, windowHeight;
			glfwGetWindowSize(window, &windowWidth, &windowHeight);

			// Normalize mouse position to OpenGL coordinates (-1 to 1)
			float x1 = 2.0f * xpos / windowWidth - 1.0f;
			float y1 = 1.0f - 2.0f * ypos / windowHeight;

			// Check "StartGameAgain" button bounds
			if (x1 > -0.2f && x1 < 0.2f && y1 > -0.1f && y1 < 0.1f) {
				cueBall.reset();
				resetCue(cueBall, cue);
				resetAllBalls(billiardBalls);
				GAME_STARTED = false;
				FIRST_BALL_HIT = false;
				FIRST_TURN_OVER = false;
				RECREATED_CUE = true;
				setCueSpeed(cueSpeed, Enums::HIT_SPEED::MEDIUM);
				gameOverOverlayVisible = false;
				nextBallNumber = 1;

				turnNumberN = 1;
				ballsPottedNumber = 0;
				ballsLeftNumber = 9;

				setBallsPotted(ballsPotted, ballsPottedNumber);
				setBallsLeft(ballsLeft, ballsLeftNumber);
				setTurnNumber(turnNumber, turnNumberN);

				BilliardBall* ballNew = findNextBall(billiardBalls);

				if (ballNew != nullptr) {
					drawNextBall(&nextBall, ballNew->color);
				}
			}

			// Check "Exit" button bounds
			if (x1 > -0.2f && x1 < 0.2f && y1 > -0.4f && y1 < -0.2f) {
				glfwSetWindowShouldClose(window, true); // Close game
			}
		}

		// 1 - Very slow speed; 2 - Slow speed; 3 - Normal speed; 4 - Fast speed; 5 - Very fast speed
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !pauseOverlayVisible) {
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::VERY_SLOW);
			cue.switchCueSpeed(Enums::HIT_SPEED::VERY_SLOW);
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !pauseOverlayVisible) {
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::SLOW);
			cue.switchCueSpeed(Enums::HIT_SPEED::SLOW);
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !pauseOverlayVisible) {
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::MEDIUM);
			cue.switchCueSpeed(Enums::HIT_SPEED::MEDIUM);
		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !pauseOverlayVisible) {
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::FAST);
			cue.switchCueSpeed(Enums::HIT_SPEED::FAST);
		}
		else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && !pauseOverlayVisible) {
			setCueSpeed(cueSpeed, Enums::HIT_SPEED::VERY_FAST);
			cue.switchCueSpeed(Enums::HIT_SPEED::VERY_FAST);
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

		// Render the cue
		cue.render();

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

		// Render the index
		for (Character& character : index) {
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

		// Render the balls potted
		for (Character& character : ballsPotted) {
			character.renderCharacter();
		}

		// Render the balls left
		for (Character& character : ballsLeft) {
			character.renderCharacter();
		}

		// Render the turn number
		for (Character& character : turnNumber) {
			character.renderCharacter();
		}

		// Render the current player
		for (Character& character : currentPlayerText) {
			character.renderCharacter();
		}

		// Render the foul text
		for (Character& character : foulText) {
			character.renderCharacter();
		}

		// End of the turn
		if (isEndOfTurn(billiardBalls)) {
			for (Character& character : nextBallText) {
				character.renderCharacter();
			}

			if (!RECREATED_CUE) {
				if (isGameOver(billiardBalls) && FOUL && !gameOverOverlayVisible) {
					gameOverOverlayVisible = true;
					std::string winner = currentPlayer == PLAYER1 ? "Player1" : "Player2";
					gameOver = drawText(winner + " you won", -0.5f, 0.5f, 0.06f, 0.1f);
				}
				else if (isGameOver(billiardBalls) && !FOUL && !gameOverOverlayVisible) {
					gameOverOverlayVisible = true;
					std::string winner = currentPlayer == PLAYER1 ? "Player2" : "Player1";
					gameOver = drawText(winner + " you won", -0.5f, 0.5f, 0.06f, 0.1f);
				}

				if (!isGameOver(billiardBalls)) {
					resetCue(cueBall, cue);
					setCueSpeed(cueSpeed, Enums::HIT_SPEED::MEDIUM);
					RECREATED_CUE = true;

					BilliardBall* ballNew = findNextBall(billiardBalls);

					if (ballNew != nullptr) {
						drawNextBall(&nextBall, ballNew->color);
					}
				}

				int currentPottedBalls = ballsPottedNumber;

				setBallsPotted(ballsPotted, findNumberOfBallsPotted(billiardBalls));
				setBallsLeft(ballsLeft, findNumberOfBallsLeft(billiardBalls) - 1);
				setTurnNumber(turnNumber, turnNumberN + 1);

				if (FOUL || findNumberOfBallsPotted(billiardBalls) == currentPottedBalls)
					setCurrentPlayer(currentPlayerText, currentPlayer == Player::PLAYER1 ? PLAYER2 : PLAYER1);
				else
					setCurrentPlayer(currentPlayerText, currentPlayer);

				if (FOUL) {
					cueBall.reset();
					resetCue(cueBall, cue);
					setFoulText(foulText, true);
				}

				FOUL = false;
				CUEBALL_VISIBLE = true;
			}

			// Render the next ball
			renderNextBall(nextBall);

		}

		// Render overlay if active
		renderOverlay(window, pauseOverlayVisible, coloredRectangles, gamePaused, continueText, exitText);

		// Render the game over overlay
		renderOverlay(window, gameOverOverlayVisible, coloredRectangles, gameOver, playAgainText, exitText);

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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		pauseOverlayVisible = !pauseOverlayVisible; // Toggle overlay visibility
	}

	if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
		gameOverOverlayVisible = !gameOverOverlayVisible; // Toggle overlay visibility FOR TESTING ONLY
	}
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS && pauseOverlayVisible) {
		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		int windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		// Normalize mouse position to OpenGL coordinates (-1 to 1)
		float x = 2.0f * xpos / windowWidth - 1.0f;
		float y = 1.0f - 2.0f * ypos / windowHeight;

		// Check "Continue" button bounds
		if (x > -0.2f && x < 0.2f && y > -0.1f && y < 0.1f) {
			pauseOverlayVisible = false; // Resume game
		}

		// Check "Exit" button bounds
		if (x > -0.2f && x < 0.2f && y > -0.4f && y < -0.2f) {
			glfwSetWindowShouldClose(window, true); // Close game
		}
	}
}

static void renderOverlay(GLFWwindow* window, bool condition, std::vector<ColoredRectangle>& rectangles, std::vector<Character>& overlay, std::vector<Character>& continueText, std::vector<Character>& exitText) {
	if (!condition) return;

	for (ColoredRectangle& rectangle : rectangles) {
		rectangle.render();
	}

	for (Character& character : overlay) {
		character.renderCharacter();
	}

	for (Character& character : continueText) {
		character.renderCharacter();
	}

	for (Character& character : exitText) {
		character.renderCharacter();
	}
}

static bool resetCue(BilliardBall& cueBall, Cue& cue) {
	cue = Cue(&cueBall, 1.f, 0.025f, 180, true, Color::BROWN);
	cue.draw("basic.vert", "basic.frag", "strides/cue.png");
	return true;
}

static void moveCueBall(Cue& cue, BilliardBall& cueBall, Enums::MOVE_DIRECTION move_direction, float move_quantity) {
	cueBall.move(move_direction, move_quantity);
	resetCue(cueBall, cue);
}

static void checkCollisionsWithBalls(BilliardBall& ball, std::vector<BilliardBall>& balls) {
	if (ball.potted) return;
	for (BilliardBall& otherBall : balls) {
		if (otherBall.potted) continue;
		if (ball.number != otherBall.number) {
			if (ball.checkCollision(&otherBall)) {
				if (ball.type == Enums::BilliardBallType::CUE && !FIRST_BALL_HIT) {
					FIRST_BALL_HIT = true;

					if (ball.number == 0 && otherBall.number != nextBallNumber && FIRST_BALL_HIT && FIRST_TURN_OVER) {
						std::cout << "FOUL" << std::endl;
						FOUL = true;
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
		if (ball.checkIfInHole(&hole)) {
			if (ball.number == 0 && CUEBALL_VISIBLE) {
				CUEBALL_VISIBLE = false;
				FOUL = true;
			}
		}
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
	cue.draw("basic.vert", "basic.frag", "strides/cue.png");
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
		if(ball.number == 0) 
			if(CUEBALL_VISIBLE)
				ball.render(dt);

		if(ball.number != 0)
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

	if (cueBall.potted) {
		return true;
		FOUL = true;
	}

	return false;
}

static void resetAllBalls(std::vector<BilliardBall>& balls) {
	balls = {
		BilliardBall(Constants::CUE_BALL_STARTING_POS_X, Constants::CUE_BALL_STARTING_POS_Y, 0.03f, Enums::BilliardBallType::CUE, Color::WHITE, 0),
		BilliardBall(0.32f, 0.0f, 0.03f, Enums::BilliardBallType::SOLID, Color::YELLOW, 1),
		BilliardBall(0.37f, 0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::BLUE, 2),
		BilliardBall(0.37f, -0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::RED, 3),
		BilliardBall(0.42f, 0.06f, 0.03f, Enums::BilliardBallType::SOLID, Color::PURPLE, 4),
		BilliardBall(0.42f, 0.0f, 0.03f, Enums::BilliardBallType::STRIPE, Color::PINK, 9),
		BilliardBall(0.42f, -0.06f, 0.03f, Enums::BilliardBallType::SOLID, Color::ORANGE, 5),
		BilliardBall(0.47f, 0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::GREEN, 6),
		BilliardBall(0.47f, -0.03f, 0.03f, Enums::BilliardBallType::SOLID, Color::BROWN, 7),
		BilliardBall(0.52f, 0.0f, 0.03f, Enums::BilliardBallType::BLACK, Color::BLACK, 8),
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
	cueSpeed = drawText("Cue speed: " + to_string(hitSpeed + 1), -0.9f, 0.91f, 0.03f, 0.05f);
}

static void hideCueSpeed(std::vector<Character>& cueSpeed) {
	cueSpeed = drawText("", -0.9f, 0.91f, 0.03f, 0.05f);
}

static void drawRectangles(std::vector<ColoredRectangle>& rectangles) {
	rectangles.at(0).draw("basic.vert", "basic.frag", "strides/space.png");

	for (int i = 1; i < rectangles.size(); i++) {
		rectangles.at(i).draw("basic.vert", "basic.frag", "characters/space.png");
	}
}

static bool isEndOfTurn(std::vector<BilliardBall>& billiardBalls) {
	return isTurnOver(billiardBalls) && FIRST_TURN_OVER && !pauseOverlayVisible;
}

static BilliardBall* findNextBall(std::vector<BilliardBall>& billiardBalls) {
	std::qsort(&billiardBalls[1], billiardBalls.size() - 1, sizeof(BilliardBall), [](const void* a, const void* b) {
		BilliardBall* ball1 = (BilliardBall*)a;
		BilliardBall* ball2 = (BilliardBall*)b;

		return ball1->number - ball2->number;
		});

	for (int i = 0; i < billiardBalls.size(); i++) {
		// Skip the cue ball
		if (billiardBalls[i].number == 0) continue;

		// If the current ball is the next ball, check if its potted and if its not, find the next ball
		if (billiardBalls[i].number == nextBallNumber) {
			// If current ball is not potted, break
			if (!billiardBalls[i].potted) return &billiardBalls[i];

			if (i == billiardBalls.size() - 1) {
				std::cout << "No more balls left!" << std::endl;
				return nullptr;
			}

			// Find the next ball that is not potted
			for (int j = i + 1; j < billiardBalls.size(); j++) {
				if (!billiardBalls[j].potted) {
					nextBallNumber = billiardBalls[j].number;
					return &billiardBalls[j];
				}
			}
		}
	}

	std::cout << "No more balls left!" << std::endl;

	return nullptr;
}

static void drawNextBall(BilliardBall** nextBall, Color color) {
	(*nextBall)->color = color;
	(*nextBall)->draw("basic.vert", "ball.frag", nullptr);
}

static void renderNextBall(BilliardBall* nextBall) {
	nextBall->render(0.0f);
}

static BilliardBall find9Ball(std::vector<BilliardBall>& balls) {
	for (BilliardBall& ball : balls) {
		if (ball.number == 9) {
			return ball;
		}
	}
}

static bool isGameOver(std::vector<BilliardBall>& balls) {
	return find9Ball(balls).potted;
}

static int findNumberOfBallsPotted(std::vector<BilliardBall>& balls) {
	int count = 0;

	for (BilliardBall& ball : balls) {
		if (ball.potted) {
			count++;
		}
	}

	return count;
}

static int findNumberOfBallsLeft(std::vector<BilliardBall>& balls) {
	int count = 0;

	for (BilliardBall& ball : balls) {
		if (!ball.potted) {
			count++;
		}
	}

	return count;
}

static void setBallsPotted(std::vector<Character>& ballsPotted, int ballsPottedN) {
	ballsPottedNumber = ballsPottedN;
	ballsPotted = drawText("Balls potted: " + to_string(ballsPottedNumber), -0.9f, 0.79f, 0.03f, 0.05f);
}

static void setBallsLeft(std::vector<Character>& ballsLeft, int ballsLeftN) {
	ballsLeftNumber = ballsLeftN;
	ballsLeft = drawText("Balls left: " + to_string(ballsLeftNumber), -0.9f, 0.73f, 0.03f, 0.05f);
}

static void setTurnNumber(std::vector<Character>& turnNumber, int turnNumberNu) {
	turnNumberN = turnNumberNu;
	turnNumber = drawText("Turn number: " + to_string(turnNumberNu), -0.9f, 0.67f, 0.03f, 0.05f);
}

static void setCurrentPlayer(std::vector<Character>& playerText, Player player) {
	playerText = drawText("Current player: " + (player == Player::PLAYER1 ? std::string("PLAYER1") : std::string("PLAYER2")), -0.9f, 0.61f, 0.03f, 0.05f);
	currentPlayer = player;
}

static void setFoulText(std::vector<Character>& foulText, bool foul) {
	foulText = drawText(foul ? "FOUL" : "", -0.1f, 0.55f, 0.05f, 0.1f);
}