#include "Image.hpp"
#ifndef CIRCLE

#include "Constants.hpp"

class Circle {
public:
    float x, y, radius; // Position and size
    unsigned int VAO, VBO, EBO, TBO; // OpenGL buffers
    unsigned int shaderProgram; // Shader program
    Image* texture;             // Texture for the pothole

    // Buffer for drawing the circle
    float circle_buf[64] = {0};

    Circle(float x, float y, float radius)
        : x(x), y(y), radius(radius), shaderProgram(0), texture(nullptr) {
        // Generate buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenBuffers(1, &TBO);
    }

    ~Circle() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        if (texture) delete texture;
    }

    virtual void draw(const char* vsSource, const char* fsSource, const char* texturePath) = 0;
    virtual void render(float dt) = 0;
};

#endif // !CIRCLE
