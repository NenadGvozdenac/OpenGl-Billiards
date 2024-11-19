#ifndef CONSTANTS_CPP
#define CONSTANTS_CPP

#include "Constants.hpp"

const char* Constants::windowName = "Billiards";
const int Constants::windowWidth = 950;
const int Constants::windowHeight = 950;
const int Constants::windowResizable = GL_FALSE;

const int Constants::NUM_SEGMENTS = 30;
const float Constants::PI = 3.14159265359f;

int Constants::CalculateWindowPositionX() {
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	if (!primaryMonitor) {
		std::cerr << "Failed to get primary monitor" << std::endl;
		return -1;
	}

	int monitorX, monitorY, monitorWidth, monitorHeight;
	glfwGetMonitorWorkarea(primaryMonitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);

	return monitorX + (monitorWidth - windowWidth) / 2;
}

int Constants::CalculateWindowPositionY() {
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	if (!primaryMonitor) {
		std::cerr << "Failed to get primary monitor" << std::endl;
		return -1;
	}

	int monitorX, monitorY, monitorWidth, monitorHeight;
	glfwGetMonitorWorkarea(primaryMonitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);

	return monitorY + (monitorHeight - windowHeight) / 2;
}

unsigned int Shader::compileShader(GLenum type, const char* source)
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

unsigned int Shader::createShader(const char* vsSource, const char* fsSource)
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

float Color::WHITE[] = { 1.0f, 1.0f, 1.0f };
float Color::BLUE[] = { 0.0f, 0.0f, 1.0f };
float Color::ORANGE[] = { 1.0f, 0.5f, 0.0f };
float Color::RED[] = { 1.0f, 0.0f, 0.0f };
float Color::GREEN[] = { 0.0f, 1.0f, 0.0f };
float Color::YELLOW[] = { 1.0f, 1.0f, 0.0f };
float Color::BLACK[] = { 0.0f, 0.0f, 0.0f };
float Color::PURPLE[] = { 0.5f, 0.0f, 0.5f };
float Color::PINK[] = { 1.0f, 0.0f, 1.0f };
float Color::BROWN[] = { 0.647f, 0.165f, 0.165f };

#endif // CONSTANTS_CPP