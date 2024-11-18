#ifndef POTHOLE_HPP
#define POTHOLE_HPP

#include "Constants.hpp"
#include "Image.hpp"

class PotHole {
public:
    float x, y, width, height; // Position and size
    unsigned int VAO, VBO, EBO; // OpenGL buffers
    unsigned int shaderProgram; // Shader program
    Image* texture;             // Texture for the pothole

    PotHole(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height), texture(nullptr) {
		// Generate buffers
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}

    ~PotHole() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        if (texture) delete texture;
    }

    void drawPotHole(const char* vsSource, const char* fsSource, const char* texturePath);
    void renderPotHole();
};

#endif // !POTHOLE_HPP
