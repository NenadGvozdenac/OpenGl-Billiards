#ifndef POTHOLE_HPP
#define POTHOLE_HPP

#include "Circle.hpp"

class PotHole : public Circle {
public:
    PotHole(float x, float y, float radius);
    void drawPotHole(const char* vsSource, const char* fsSource, const char* texturePath);
    void renderPotHole();
};

#endif // !POTHOLE_HPP
