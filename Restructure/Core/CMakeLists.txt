#
# Core
#

set(SOURCES
#stb_image
vendor/stb/stb_image.h
vendor/stb/stb_image.cpp

#glad
vendor/glad/include/glad/glad.h
vendor/glad/src/glad.c
vendor/glad/include/KHR/khrplatform.h

#imgui Includes
vendor/ImGui/imconfig.h
vendor/ImGui/imgui.cpp
vendor/ImGui/imgui.h
vendor/ImGui/imgui_demo.cpp
vendor/ImGui/imgui_draw.cpp
vendor/ImGui/imgui_impl_glfw.cpp
vendor/ImGui/imgui_impl_glfw.h
vendor/ImGui/imgui_impl_opengl3.cpp
vendor/ImGui/imgui_impl_opengl3.h
vendor/ImGui/imgui_internal.h
vendor/ImGui/imgui_tables.cpp
vendor/ImGui/imgui_widgets.cpp
vendor/ImGui/imstb_rectpack.h
vendor/ImGui/imstb_textedit.h
vendor/ImGui/imstb_truetype.h

#application and engine
Source/Core/Application.h
Source/Core/Application.cpp
Source/Core/Window.h
Source/Core/Window.cpp
Source/Core/Layer.h
Source/Core/InputManager.cpp
Source/Core/InputManager.h
Source/Core/ResourceManager.h
Source/Core/ResourceManager.cpp
Source/Core/ImGuiLayer.h
Source/Core/ImGuiLayer.cpp
Source/Core/Renderer/Renderer.h
Source/Core/Renderer/Renderer.cpp
Source/Core/Renderer/Shader.h
Source/Core/Renderer/Shader.cpp
Source/Core/Renderer/Texture.h
Source/Core/Renderer/Texture.cpp
Source/Core/Renderer/Camera.h

)

add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(Core glfw)
target_link_libraries(Core glm)

target_include_directories(Core PUBLIC "Source" "vendor/stb" "vendor/glad/include" "vendor/ImGui")


add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/App/Shaders
    ${PROJECT_BINARY_DIR}/App/Shaders
    COMMENT "Copying resources into binary directory")
