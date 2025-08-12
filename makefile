BASE_OBJS = GLEngine.o game.o Shader.o Texture.o resource_manager.o LevelLayout.o glad.o
CXX = g++
CXXFLAGS = -I/home/Jarod/Desktop/LearnOpenGL/GL_Engine/src/include
SRC_PATH = src

OBJS = $(addprefix $(SRC_PATH)/, $(BASE_OBJS))
LDLIBS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp

GLEngine: $(OBJS)
	$(CXX) $(CXXFLAGS) -o GLEngine.app $^ $(LDLIBS)

clean:
	rm -f $(OBJS)
