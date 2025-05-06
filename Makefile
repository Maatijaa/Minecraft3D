CXX := g++
CXXFLAGS := -std=c++17 -Wall -Isrc -I./imgui -I./imgui/backends $(shell sdl2-config --cflags)
LDLIBS := -lSDL2 -lSDL2_image -lGL -lGLU

TARGET := build/ParadoxersMinecraftEngine

SRC := src/main.cpp src/InputController.cpp src/core/Terrain.cpp \
       imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp \
       imgui/imgui_widgets.cpp \
       imgui/backends/imgui_impl_sdl2.cpp imgui/backends/imgui_impl_opengl2.cpp

OBJ := $(patsubst %.cpp, build/%.o, $(SRC))

all: create_build_dir $(TARGET)

create_build_dir:
	mkdir -p build

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDLIBS)

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build