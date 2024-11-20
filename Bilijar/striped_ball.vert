#version 330 core

// Input attributes
layout(location = 0) in vec2 aPos; // Vertex position

// Outputs to the fragment shader
out float fragY; // Y position for stripe logic

void main() {
    // Pass Y position to the fragment shader
    fragY = aPos.y;

    // Set final vertex position
    gl_Position = vec4(aPos, 0.0, 1.0);
}
