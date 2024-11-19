#define _CRT_SECURE_NO_WARNINGS

#include "Constants.hpp"

#include "BilliardTable.hpp"
#include "TableEdge.hpp"
#include "BilliardBall.hpp"
#include "Cue.hpp"

using namespace std;

const bool DISPLAY_EDGES = false;

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

    TableEdge leftUpperEdge(-0.645f, 0.365f, -0.67f, 0.395f, -0.0325f, 0.395f, -0.0325f, 0.365f, TableEdgeType::TOP, DISPLAY_EDGES);
    leftUpperEdge.drawEdge("basic.vert", "edgefrag.frag");

    TableEdge rightUpperEdge(0.0465f, 0.365f, 0.0465f, 0.395f, 0.685f, 0.395f, 0.655f, 0.365f, TableEdgeType::TOP, DISPLAY_EDGES);
    rightUpperEdge.drawEdge("basic.vert", "edgefrag.frag");

    TableEdge leftLowerEdge(-0.645f, -0.355f, -0.67f, -0.385f, -0.0325f, -0.385f, -0.0325f, -0.355f, TableEdgeType::BOTTOM, DISPLAY_EDGES);
    leftLowerEdge.drawEdge("basic.vert", "edgefrag.frag");

    TableEdge rightLowerEdge(0.0465f, -0.355f, 0.0465f, -0.385f, 0.685f, -0.385f, 0.655f, -0.355f, TableEdgeType::BOTTOM, DISPLAY_EDGES);
    rightLowerEdge.drawEdge("basic.vert", "edgefrag.frag");

    TableEdge leftEdge(-0.725f, -0.343f, -0.725f, 0.35f, -0.695f, 0.32f, -0.695f, -0.308f, TableEdgeType::LEFT, DISPLAY_EDGES);
    leftEdge.drawEdge("basic.vert", "edgefrag.frag");

    TableEdge rightEdge(0.733f, -0.343f, 0.733f, 0.35f, 0.703f, 0.32f, 0.703f, -0.308f, TableEdgeType::RIGHT, DISPLAY_EDGES);
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

    BilliardBall cueBall(-0.45f, 0.0f, 0.03f, BilliardBallType::CUE, Color::WHITE, 0);
    cueBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall solidYellowBall(0.32f, 0.0f, 0.03f, BilliardBallType::SOLID, Color::YELLOW, 0);
    solidYellowBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall solidRedBall(0.37f, 0.03f, 0.03f, BilliardBallType::SOLID, Color::RED, 0);
    solidRedBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall stripedBlueBall(0.37f, -0.03f, 0.03f, BilliardBallType::STRIPE, Color::BLUE, 0);
    stripedBlueBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall stripedOrangeBall(0.42f, 0.06f, 0.03f, BilliardBallType::STRIPE, Color::ORANGE, 0);
    stripedOrangeBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall blackBall(0.42f, 0.0f, 0.03f, BilliardBallType::BLACK, Color::BLACK, 0);
    blackBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall solidGreenBall(0.42f, -0.06f, 0.03f, BilliardBallType::SOLID, Color::GREEN, 0);
    solidGreenBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall solidBlueBall(0.47f, 0.09f, 0.03f, BilliardBallType::SOLID, Color::BLUE, 0);
    solidBlueBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall stripedBrownBall(0.47f, 0.03f, 0.03f, BilliardBallType::STRIPE, Color::BROWN, 0);
    stripedBrownBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall solidPurpleBall(0.47f, -0.03f, 0.03f, BilliardBallType::SOLID, Color::PURPLE, 0);
    solidPurpleBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall stripedYellowBall(0.47f, -0.09f, 0.03f, BilliardBallType::STRIPE, Color::YELLOW, 0);
    stripedYellowBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall solidBrownBall(0.52f, 0.12f, 0.03f, BilliardBallType::SOLID, Color::BROWN, 0);
    solidBrownBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall stripedPurpleBall(0.52f, 0.06f, 0.03f, BilliardBallType::STRIPE, Color::PURPLE, 0);
    stripedPurpleBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall solidOrangeBall(0.52f, 0.0f, 0.03f, BilliardBallType::SOLID, Color::ORANGE, 0);
    solidOrangeBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall stripedRedBall(0.52f, -0.06f, 0.03f, BilliardBallType::STRIPE, Color::RED, 0);
    stripedRedBall.draw("basic.vert", "ball.frag", nullptr);

    BilliardBall stripedGreenBall(0.52f, -0.12f, 0.03f, BilliardBallType::STRIPE, Color::GREEN, 0);
    stripedGreenBall.draw("basic.vert", "ball.frag", nullptr);

    Cue cue(&cueBall, 1.f, 0.025f, 240, true, Color::BROWN);
    cue.draw("basic.vert", "ball.frag", nullptr);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
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
        potHole1.render();
        potHole2.render();
        potHole3.render();
        potHole4.render();
        potHole5.render();
        potHole6.render();

        // Render the cue ball
        cueBall.render();

        // Render the other balls
        blackBall.render();
        solidYellowBall.render();
        stripedOrangeBall.render();
        solidOrangeBall.render();
        solidRedBall.render();
        stripedBlueBall.render();
        solidGreenBall.render();
        solidBlueBall.render();
        stripedBrownBall.render();
        solidPurpleBall.render();
        stripedYellowBall.render();
        solidBrownBall.render();
        stripedPurpleBall.render();
        solidOrangeBall.render();
        stripedRedBall.render();
        stripedGreenBall.render();

        // Render the cue
        cue.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}