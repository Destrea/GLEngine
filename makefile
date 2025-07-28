BASE_OBJS = GLEngine.o game.o Shader.o Texture.o Mesh.o Model.o resource_manager.o glad.o
CXX = g++
CXXFLAGS = -I/home/Destrea/Desktop/LearnOpenGL/include
SRC_PATH = src

OBJS = $(addprefix $(SRC_PATH)/, $(BASE_OBJS))
LDLIBS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp

GLEngine: $(OBJS)
	$(CXX) $(CXXFLAGS) -o GLEngine $^ $(LDLIBS)


clean:
	rm -f *(OBJS)
