
cmake_minimum_required(VERSION 3.15...4.0)

project(GLEngine    VERSION 0.2
                    DESCRIPTION "A Custom OpenGL Game Engine."
                    LANGUAGES CXX C)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(OpenGL_GL_PREFERENCE GLVND)

endif()

include(FetchContent)

#adds glfw3
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1)
FetchContent_MakeAvailable(glm)

#FetchContent_Declare(
#    assimp
#    GIT_REPOSITORY https://github.com/assimp/assimp.git
#    GIT_TAG v6.0.2)
#FetchContent_MakeAvailable(assimp)
#Un-comment assimp once mesh.h/model.h are used or needed.

add_executable(${PROJECT_NAME} "src/GLEngine.cpp")

target_compile_features(GLEngine PUBLIC cxx_std_11)

#Source code is here
add_subdirectory(src)
add_subdirectory(include)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources
    ${PROJECT_BINARY_DIR}/resources
    COMMENT "Copying resources into binary directory")
configure_file(${PROJECT_SOURCE_DIR}/imgui.ini ${PROJECT_BINARY_DIR} COPYONLY)


target_include_directories(${PROJECT_NAME} PRIVATE "{CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../extern/ImGui)
target_include_directories(${PROJECT_NAME} PUBLIC ../include/glad)
target_include_directories(${PROJECT_NAME} PUBLIC ../include/KHR)

target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

if(UNIX OR APPLE)
    find_package(X11)
    if(X11_FOUND)
        message("Found X11")
        target_link_libraries(GLEngine PUBLIC ${X11})
    endif()
endif(UNIX OR APPLE)

