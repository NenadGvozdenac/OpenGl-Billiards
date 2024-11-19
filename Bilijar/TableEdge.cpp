#ifndef TABLE_EDGE_CPP
#define TABLE_EDGE_CPP

#include "Constants.hpp"
#include "TableEdge.hpp"

TableEdge::TableEdge(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, TableEdgeType type)
	: Rectangle(x1, y1, x2, y2, x3, y3, x4, y4, shaderProgram), type(type), visible(true) {
}

TableEdge::TableEdge(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, TableEdgeType type, bool visible)
	: Rectangle(x1, y1, x2, y2, x3, y3, x4, y4, shaderProgram), type(type), visible(visible) {
}

void TableEdge::drawEdge(const char* vsSource, const char* fsSource) {
	shaderProgram = Shader::createShader(vsSource, fsSource);

	GLuint indices[] = {
		0, 1, 2,   // First triangle
		0, 2, 3    // Second triangle
	};

	float vertices[] = {
		x1, y1, 0.0f, 1.0f,   // Top-left
		x2, y2, 0.0f, 0.0f,   // Bottom-left
		x3, y3, 1.0f, 0.0f,   // Bottom-right
		x4, y4, 1.0f, 1.0f    // Top-right
	};

	unsigned int stride = (2 + 2) * sizeof(float);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void TableEdge::renderEdge() {
	if (!visible) return;

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

#endif