BASE_OBJS = GLEngine.o game.o Shader.o Texture.o resource_manager.o LevelLayout.o glad.o imgui.o imgui_demo.o imgui_draw.o imgui_impl_glfw.o imgui_impl_opengl3.o imgui_tables.o imgui_widgets.o ImguiLayer.o FramebufferHandler.o InputManager.o
CXX = g++
CXXFLAGS = -I/home/Jarod/Desktop/GL_Engine/src/include -I/home/Jarod/Desktop/GL_Engine/src/include/ImGUI
SRC_PATH = src

OBJS = $(addprefix $(SRC_PATH)/, $(BASE_OBJS))
LDLIBS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp

GLEngine: $(OBJS)
	$(CXX) $(CXXFLAGS) -o GLEngine.app $^ $(LDLIBS)

clean:
	rm -f $(OBJS)
