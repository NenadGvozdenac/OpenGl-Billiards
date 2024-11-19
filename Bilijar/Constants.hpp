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

	static const int NUM_SEGMENTS;
	static const float PI;
};

class Shader {
public:
	static unsigned int compileShader(GLenum type, const char* source)
	{
		std::string content = "";
		std::ifstream file(source);
		std::stringstream ss;
		if (file.is_open())
		{
			ss << file.rdbuf();
			file.close();
			std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
		}
		else {
			ss << "";
			std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
		}
		std::string temp = ss.str();
		const char* sourceCode = temp.c_str();

		int shader = glCreateShader(type);

		int success;
		char infoLog[512];
		glShaderSource(shader, 1, &sourceCode, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			if (type == GL_VERTEX_SHADER)
				printf("VERTEX");
			else if (type == GL_FRAGMENT_SHADER)
				printf("FRAGMENT");
			printf(" sejder ima gresku! Greska: \n");
			printf(infoLog);
		}
		return shader;
	}

	static unsigned int createShader(const char* vsSource, const char* fsSource)
	{

		unsigned int program;
		unsigned int vertexShader;
		unsigned int fragmentShader;

		program = glCreateProgram();

		vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
		fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);


		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);
		glValidateProgram(program);

		int success;
		char infoLog[512];
		glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(program, 512, NULL, infoLog);
			std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
			std::cout << infoLog << std::endl;
		}

		glDetachShader(program, vertexShader);
		glDeleteShader(vertexShader);
		glDetachShader(program, fragmentShader);
		glDeleteShader(fragmentShader);

		return program;
	}
};

#endif // CONSTANTS_HPP