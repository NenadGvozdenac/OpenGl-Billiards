#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Constants {
public:
	// Constants for the window
	static const char* windowName;
	static const int windowWidth;
	static const int windowHeight;
	static const int windowResizable;

    static int CalculateWindowPositionX() {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        if (!primaryMonitor) {
            std::cerr << "Failed to get primary monitor" << std::endl;
            return -1;
        }

        int monitorX, monitorY, monitorWidth, monitorHeight;
        glfwGetMonitorWorkarea(primaryMonitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);

        return monitorX + (monitorWidth - windowWidth) / 2;
    }

    static int CalculateWindowPositionY() {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        if (!primaryMonitor) {
            std::cerr << "Failed to get primary monitor" << std::endl;
            return -1;
        }

        int monitorX, monitorY, monitorWidth, monitorHeight;
        glfwGetMonitorWorkarea(primaryMonitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);

        return monitorY + (monitorHeight - windowHeight) / 2;
    }
};

#endif // CONSTANTS_HPP