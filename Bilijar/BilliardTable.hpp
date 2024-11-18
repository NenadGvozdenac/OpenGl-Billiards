#ifndef BILLIARD_TABLE_HPP
#define BILLIARD_TABLE_HPP

#include "Constants.hpp"
#include <vector>
#include "Pothole.hpp"

class BilliardTable {
public:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int shaderProgram;
    Image* texture;
    std::vector<PotHole> potHoles;

    BilliardTable()
        : VAO(0), VBO(0), EBO(0), shaderProgram(0), texture(nullptr) {}

    ~BilliardTable() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        if (texture) delete texture;

        for (auto& potHole : potHoles) {
			potHole.~PotHole();
		}
    }

    void drawTable(const char* vsSource, const char* fsSource, const char* texturePath);
    void renderTable();
};

#endif // BILLIARD_TABLE_HPP
