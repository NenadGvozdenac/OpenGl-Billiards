#define _CRT_SECURE_NO_WARNINGS

#include "Constants.hpp"

#include "BilliardTable.hpp"

using namespace std;

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
    
    PotHole potHole1(0.725f, 0.39f, 0.04f);
    potHole1.drawPotHole("basic.vert", "circle.frag", nullptr);

    PotHole potHole2(-0.713f, 0.39f, 0.04f);
    potHole2.drawPotHole("basic.vert", "circle.frag", nullptr);

    PotHole potHole3(0.725f, -0.376f, 0.04f);
    potHole3.drawPotHole("basic.vert", "circle.frag", nullptr);

    PotHole potHole4(-0.715f, -0.378f, 0.04f);
    potHole4.drawPotHole("basic.vert", "circle.frag", nullptr);

    PotHole potHole5(0.005f, 0.42f, 0.04f);
    potHole5.drawPotHole("basic.vert", "circle.frag", nullptr);

    PotHole potHole6(0.005f, -0.405f, 0.04f);
    potHole6.drawPotHole("basic.vert", "circle.frag", nullptr);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the table
        table.renderTable();

        // Render the pothole
        potHole1.renderPotHole();
        potHole2.renderPotHole();
        potHole3.renderPotHole();
        potHole4.renderPotHole();
        potHole5.renderPotHole();
        potHole6.renderPotHole();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}