#include "Terrain.hpp"
#include <cmath>
#include <cstdlib>
#define STB_PERLIN_IMPLEMENTATION
#include "libs/stb_perlin.h"

Terrain::Terrain(int width, int depth, float blockSize, GLuint texture)
    : width(width), depth(depth), blockSize(blockSize), texture(texture) {}

void Terrain::drawCube(float x, float y, float z, float size, GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);

    // Top
    glTexCoord2f(0, 0); glVertex3f(x, y + size, z);
    glTexCoord2f(1, 0); glVertex3f(x + size, y + size, z);
    glTexCoord2f(1, 1); glVertex3f(x + size, y + size, z + size);
    glTexCoord2f(0, 1); glVertex3f(x, y + size, z + size);

    // Bottom
    glTexCoord2f(0, 0); glVertex3f(x, y, z);
    glTexCoord2f(1, 0); glVertex3f(x + size, y, z);
    glTexCoord2f(1, 1); glVertex3f(x + size, y, z + size);
    glTexCoord2f(0, 1); glVertex3f(x, y, z + size);

    // Front
    glTexCoord2f(0, 0); glVertex3f(x, y, z + size);
    glTexCoord2f(1, 0); glVertex3f(x + size, y, z + size);
    glTexCoord2f(1, 1); glVertex3f(x + size, y + size, z + size);
    glTexCoord2f(0, 1); glVertex3f(x, y + size, z + size);

    // Back
    glTexCoord2f(0, 0); glVertex3f(x, y, z);
    glTexCoord2f(1, 0); glVertex3f(x + size, y, z);
    glTexCoord2f(1, 1); glVertex3f(x + size, y + size, z);
    glTexCoord2f(0, 1); glVertex3f(x, y + size, z);

    // Left
    glTexCoord2f(0, 0); glVertex3f(x, y, z);
    glTexCoord2f(1, 0); glVertex3f(x, y, z + size);
    glTexCoord2f(1, 1); glVertex3f(x, y + size, z + size);
    glTexCoord2f(0, 1); glVertex3f(x, y + size, z);

    // Right
    glTexCoord2f(0, 0); glVertex3f(x + size, y, z);
    glTexCoord2f(1, 0); glVertex3f(x + size, y, z + size);
    glTexCoord2f(1, 1); glVertex3f(x + size, y + size, z + size);
    glTexCoord2f(0, 1); glVertex3f(x + size, y + size, z);

    glEnd();
}

float Terrain::getHeight(int x, int z) {
    float scale = 0.1f;
    float noise = stb_perlin_noise3(x * scale, 0, z * scale, 0, 0, 0);
    float height = (noise + 1.0f) / 2.0f;
    return height * 10.0f;
}

void Terrain::render() {
    for (int x = 0; x < width; ++x) {
        for (int z = 0; z < depth; ++z) {
            int height = static_cast<int>(getHeight(x, z));

            for (int y = 0; y <= height; ++y) {
                drawCube(x * blockSize, y * blockSize, z * blockSize, blockSize, texture);
            }
        }
    }
}