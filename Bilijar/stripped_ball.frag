#version 330 core

out vec4 outCol;

// Inputs from vertex shader
in float fragY;

// Uniforms for colors
uniform vec3 ballColor;   // Base ball color
uniform vec3 stripeColor; // Stripe color

// Stripe configuration
const float stripeWidth = 0.2; // Stripe width (adjustable)

void main() {
    // Check if the current fragment lies in the stripe
    if (abs(fragY) < stripeWidth) {
        outCol = vec4(stripeColor, 1.0); // Render stripe color
    } else {
        outCol = vec4(ballColor, 1.0);   // Render ball color
    }
}
