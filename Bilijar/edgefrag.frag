#version 330 core

in vec2 chTex; // Texture coordinates (not used here)
out vec4 outCol; // Final output color

void main()
{
    outCol = vec4(0.0, 1.0, 0.0, 1.0); // Solid green color (R, G, B, A)
}