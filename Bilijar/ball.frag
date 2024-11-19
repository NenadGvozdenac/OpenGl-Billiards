#version 330 core

out vec4 outCol;

// Uniform for color
uniform vec3 ballColor;

void main()
{
    outCol = vec4(ballColor, 1.0); // Use the color passed in from the application
}
