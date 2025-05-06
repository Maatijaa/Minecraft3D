#include "InputController.hpp"
#include <cmath>

void InputController::handleInput(const Uint8* keystate) {
    float dx = 0.0f, dz = 0.0f;

    if (keystate[SDL_SCANCODE_W]) dz -= speed;
    if (keystate[SDL_SCANCODE_S]) dz += speed;
    if (keystate[SDL_SCANCODE_A]) dx -= speed;
    if (keystate[SDL_SCANCODE_D]) dx += speed;
    if (keystate[SDL_SCANCODE_SPACE]) camY += speed;
    if (keystate[SDL_SCANCODE_LCTRL]) camY -= speed;
    if (keystate[SDL_SCANCODE_ESCAPE]) {
        shouldQuit = true;
    }

    float rad = yaw * M_PI / 180.0f;
    camX += dx * cos(rad) - dz * sin(rad);
    camZ += dz * cos(rad) + dx * sin(rad);
}

void InputController::handleMouse(int dx, int dy) {
    yaw += dx * sensitivity;
    pitch -= dy * sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}