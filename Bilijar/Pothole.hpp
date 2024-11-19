#ifndef POTHOLE_HPP
#define POTHOLE_HPP

#include "Circle.hpp"

class PotHole : public Circle {
public:
    PotHole(float x, float y, float radius);
    void draw(const char* vsSource, const char* fsSource, const char* texturePath);
    void render();
};

#endif // !POTHOLE_HPP
