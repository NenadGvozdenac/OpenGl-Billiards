#ifndef IMAGE_CPP
#define IMAGE_CPP

#include "Constants.hpp"
#include "Image.hpp"

//stb_image.h je header-only biblioteka za ucitavanje tekstura.
//Potrebno je definisati STB_IMAGE_IMPLEMENTATION prije njenog ukljucivanja
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Image::loadImage() {
    std::cout << "Loading image: " << filePath << std::endl;

    unsigned char* imageData = stbi_load(filePath, &width, &height, &channels, 0);
    if (imageData == nullptr) {
        std::cerr << "Failed to load image: " << filePath << std::endl;
        return false;
    }

    // Flip image vertically to match OpenGL's bottom-left origin
    stbi__vertical_flip(imageData, width, height, channels);

    // Determine the internal format
    GLint internalFormat = (channels == 4) ? GL_RGBA : (channels == 3) ? GL_RGB : GL_RGB;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, imageData);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Generate mipmaps for better scaling
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Free the image data now that the texture has been created
    stbi_image_free(imageData);

    std::cout << "Image loaded successfully. Texture ID: " << textureID
        << ", Width: " << width << ", Height: " << height
        << ", Channels: " << channels << std::endl;

    return true;
}

#endif // IMAGE_CPP