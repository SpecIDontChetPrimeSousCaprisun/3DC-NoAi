CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
TARGET = game

SRC = src/main.cpp       \
      src/glad.c         \
      src/Window.cpp     \
      src/Shader.cpp     \
      src/Node.cpp       \
      src/Mesh.cpp       \
      src/Camera.cpp     \
      src/PointLight.cpp \
      src/stbImpl.cpp    \
      src/Texture.cpp	 \
      src/Player.cpp     \
      src/State.cpp      \
      src/IdleState.cpp

OBJ_DIR = build

OBJ = $(SRC:src/%.cpp=$(OBJ_DIR)/src/%.o)
OBJ := $(OBJ:src/glad.c=$(OBJ_DIR)/src/glad.o)

LIBS_LINUX = -lglfw    \
	     -lGL      \
	     -ldl      \
	     -lX11     \
	     -lXrandr  \
	     -lXi      \
	     -lXxf86vm \
	     -lXcursor \
	     -lassimp

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LIBS_LINUX)

$(OBJ_DIR)/src/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/src/glad.o: src/glad.c
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET)
	rm -rf publish

test: all
	gdb -q -batch -x debug.gdb --args ./$(TARGET)

.PHONY: all clean test windows publish
