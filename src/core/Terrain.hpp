#pragma once
#include <GL/gl.h>

class Terrain {
public:
    Terrain(int width, int depth, float blockSize, GLuint texture);
    void render();

private:
    int width, depth;
    float blockSize;
    GLuint texture;

    void drawCube(float x, float y, float z, float size, GLuint texture);
    float getHeight(int x, int z);
};