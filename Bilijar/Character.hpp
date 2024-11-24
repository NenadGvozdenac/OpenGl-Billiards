#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "Constants.hpp"
#include "Image.hpp"

class Character {
	unsigned int VAO, VBO, EBO;
	unsigned int shaderProgram;
	float x, y, width, height;

    Image* texture;
public:
	Character(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}
	
    void drawCharacter(const char* vsSource, const char* fsSource, const char* texturePath) {
        shaderProgram = Shader::createShader(vsSource, fsSource);

        float vertices[] = {
            // positions        // texture coords
            x,          y,          0.0f, 0.0f, // bottom-left
            x + width,  y,          1.0f, 0.0f, // bottom-right
            x,          y + height, 0.0f, 1.0f, // top-left
            x + width,  y + height, 1.0f, 1.0f  // top-right
        };

        unsigned int indices[] = { 0, 1, 2, 1, 2, 3 };

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

		texture = new Image(texturePath);
		texture->loadImage();
    }

    void renderCharacter() {
        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

        glUniform1i(glGetUniformLocation(shaderProgram, "text"), 0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }
};

#endif