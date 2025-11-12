#
# Core
#

set(SOURCES
vendor/stb/stb_image.h
vendor/stb/stb_image.cpp
vendor/glad/include/glad/glad.h
vendor/glad/src/glad.c
vendor/glad/include/KHR/khrplatform.h
Source/Core/Application.h
Source/Core/Application.cpp
Source/Core/Window.h
Source/Core/Window.cpp
Source/Core/Layer.h
Source/Core/InputManager.cpp
Source/Core/InputManager.h
Source/Core/ResourceManager.h
Source/Core/ResourceManager.cpp
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

target_include_directories(Core PUBLIC "Source" "vendor/stb" "vendor/glad/include")


add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/App/Shaders
    ${PROJECT_BINARY_DIR}/App/Shaders
    COMMENT "Copying resources into binary directory")
