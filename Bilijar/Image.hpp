#ifndef IMAGE_HPP
#define IMAGE_HPP

class Image {
public:
    // Constructor to initialize the file path
    Image(const char* filePath) : filePath(filePath), textureID(0), width(0), height(0), channels(0) {}

    // Load the image and generate the texture
    bool loadImage();

    // Getters for image properties
    unsigned int getTextureID() const { return textureID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
private:
    const char* filePath; // Path to the image file
    unsigned int textureID; // OpenGL texture ID
    int width, height, channels; // Image properties: dimensions and number of channels
};

#endif // IMAGE_HPP