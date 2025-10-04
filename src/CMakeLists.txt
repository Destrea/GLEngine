
project(GLEngine)
set(SOURCES "glad.c" "FramebufferHandler.cpp" "game.cpp" "GLEngine.cpp" "LevelLayout.cpp" "LevelEditor.cpp" "resource_manager.cpp" "Shader.cpp" "Texture.cpp" "InputManager.cpp" "ImguiLayer.cpp")
#set(HEADER_LIST "${GLEngine_SOURCE_DIR}/include")





#add_library(glad glad.c ../include/glad/glad.h)
add_library(imgui   ../extern/ImGui/imconfig.h ../extern/ImGui/imgui.cpp ../extern/ImGui/imgui.h ../extern/ImGui/imgui_draw.cpp ../extern/ImGui/imgui_impl_glfw.cpp ../extern/ImGui/imgui_impl_glfw.h
                    ../extern/ImGui/imgui_impl_opengl3.cpp ../extern/ImGui/imgui_impl_opengl3.h ../extern/ImGui/imgui_impl_opengl3_loader.h ../extern/ImGui/imgui_internal.h
                    ../extern/ImGui/imgui_tables.cpp ../extern/ImGui/imgui_widgets.cpp ../extern/ImGui/imstb_rectpack.h ../extern/ImGui/imstb_textedit.h ../extern/ImGui/imstb_truetype.h)





#target_link_libraries(engine_lib PRIVATE glfw)

#target_compile_features(engine_lib PUBLIC cxx_std_11)



#set(CMAKE_EXE_LINKER_FLAGS "-lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp")

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)
#find_package(OpenGL REQUIRED)
#find_package(glfw REQUIRED)
#find_package(glad REQUIRED)



target_sources(${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS})
#target_link_libraries(${PROJECT_NAME} PUBLIC engine_lib)









