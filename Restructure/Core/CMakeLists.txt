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
Source/Core/Renderer/Renderer.h
Source/Core/Renderer/Renderer.cpp
Source/Core/Renderer/Shader.h
Source/Core/Renderer/Shader.cpp
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
