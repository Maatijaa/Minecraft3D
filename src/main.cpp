#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>

#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_sdl2.h"
#include "../imgui/backends/imgui_impl_opengl2.h"

#include "InputController.hpp"
#include "core/Terrain.hpp"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

void drawCube(float x, float y, float z, float size, GLuint textureID) {
    float half = size / 2.0f;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 0); glVertex3f(x - half, y - half, z + half);
    glTexCoord2f(1, 0); glVertex3f(x + half, y - half, z + half);
    glTexCoord2f(1, 1); glVertex3f(x + half, y + half, z + half);
    glTexCoord2f(0, 1); glVertex3f(x - half, y + half, z + half);

    glTexCoord2f(0, 0); glVertex3f(x + half, y - half, z - half);
    glTexCoord2f(1, 0); glVertex3f(x - half, y - half, z - half);
    glTexCoord2f(1, 1); glVertex3f(x - half, y + half, z - half);
    glTexCoord2f(0, 1); glVertex3f(x + half, y + half, z - half);

    glTexCoord2f(0, 0); glVertex3f(x - half, y + half, z - half);
    glTexCoord2f(1, 0); glVertex3f(x - half, y + half, z + half);
    glTexCoord2f(1, 1); glVertex3f(x + half, y + half, z + half);
    glTexCoord2f(0, 1); glVertex3f(x + half, y + half, z - half);

    glTexCoord2f(0, 0); glVertex3f(x - half, y - half, z - half);
    glTexCoord2f(1, 0); glVertex3f(x + half, y - half, z - half);
    glTexCoord2f(1, 1); glVertex3f(x + half, y - half, z + half);
    glTexCoord2f(0, 1); glVertex3f(x - half, y - half, z + half);

    glTexCoord2f(0, 0); glVertex3f(x - half, y - half, z - half);
    glTexCoord2f(1, 0); glVertex3f(x - half, y - half, z + half);
    glTexCoord2f(1, 1); glVertex3f(x - half, y + half, z + half);
    glTexCoord2f(0, 1); glVertex3f(x - half, y + half, z - half);

    glTexCoord2f(0, 0); glVertex3f(x + half, y - half, z + half);
    glTexCoord2f(1, 0); glVertex3f(x + half, y - half, z - half);
    glTexCoord2f(1, 1); glVertex3f(x + half, y + half, z - half);
    glTexCoord2f(0, 1); glVertex3f(x + half, y + half, z + half);

    glEnd();
}

GLuint loadTexture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cerr << "Failed to load texture: " << path << "\nReason: " << IMG_GetError() << std::endl;
        exit(1);
    }

    SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface);

    if (!converted) {
        std::cerr << "Failed to convert surface: " << SDL_GetError() << std::endl;
        exit(1);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, converted->w, converted->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_FreeSurface(converted);
    return textureID;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL couldn't initialize: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image couldn't initialize PNG: " << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Paradoxer's Minecraft",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        std::cerr << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();

    SDL_SetRelativeMouseMode(SDL_TRUE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (double)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    GLuint texture = loadTexture("textures/grasstexture.png");
    Terrain terrain(16, 16, 1.0f, texture);
    InputController input;

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_MOUSEMOTION && !io.WantCaptureMouse)
                input.handleMouse(event.motion.xrel, event.motion.yrel);
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (!io.WantCaptureKeyboard)
            input.handleInput(keystate);

        if (input.shouldQuit)
            running = false;

        glClearColor(0.5f, 0.75f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        glRotatef(input.pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(input.yaw, 0.0f, 1.0f, 0.0f);
        glTranslatef(-input.camX, -input.camY, -input.camZ);

        terrain.render();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Debug Menu");
        ImGui::Text("Pitch: %.1f", input.pitch);
        ImGui::Text("Yaw: %.1f", input.yaw);
        ImGui::Text("Position: (%.1f, %.1f, %.1f)", input.camX, input.camY, input.camZ);
        static float fps = 0.0f;
        fps = 1.0f / ImGui::GetIO().DeltaTime;
        ImGui::Text("FPS: %.1f", fps);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}