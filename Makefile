CXX := g++
CXXFLAGS := -std=c++17 -Wall -Isrc -I./imgui -I./imgui/backends $(shell sdl2-config --cflags)

LDLIBS := -lSDL2 -lSDL2_image -lGL -lGLU

TARGET := ParadoxersMinecraftEngine

SRC := src/main.cpp src/InputController.cpp src/core/Terrain.cpp \
       imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp \
       imgui/imgui_widgets.cpp \
       imgui/backends/imgui_impl_sdl2.cpp imgui/backends/imgui_impl_opengl2.cpp

OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)