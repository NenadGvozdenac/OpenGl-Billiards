#include "Image.hpp"
#ifndef CIRCLE

#include "Constants.hpp"

class Circle {
public:
    float x, y, radius; // Position and size
    unsigned int VAO, VBO, EBO; // OpenGL buffers
    unsigned int shaderProgram; // Shader program
    Image* texture;             // Texture for the pothole

    Circle(float x, float y, float radius)
        : x(x), y(y), radius(radius), texture(nullptr) {
        // Generate buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }

    ~Circle() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        if (texture) delete texture;
    }

    virtual void drawPotHole(const char* vsSource, const char* fsSource, const char* texturePath) = 0;
    virtual void renderPotHole() = 0;
};

#endif // !CIRCLE
