#ifndef INPUT_CONTROLLER_HPP
#define INPUT_CONTROLLER_HPP

#include <SDL2/SDL.h>

class InputController {
public:
    float camX = 0.0f, camY = 0.0f, camZ = 5.0f;
    float yaw = 0.0f, pitch = 0.0f;
    float speed = 0.1f;
    float sensitivity = 0.1f;
    bool shouldQuit = false;

    void handleInput(const Uint8* keystate);
    void handleMouse(int dx, int dy);
};

#endif