
cmake_minimum_required(VERSION 3.15...4.0)

project(GLEngine    VERSION 0.2
                    DESCRIPTION "A Custom OpenGL Game Engine."
                    LANGUAGES CXX C)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(OpenGL_GL_PREFERENCE GLVND)

endif()


add_executable(${PROJECT_NAME} "src/GLEngine.cpp")

target_compile_features(GLEngine PUBLIC cxx_std_11)

#Source code is here
add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(extern/glfw-3.4)
add_subdirectory(extern/glm-1.0.1)

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


find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

if(UNIX OR APPLE)
    find_package(X11)
    if(X11_FOUND)
        message("Found X11")
        target_link_libraries(GLEngine PUBLIC ${X11})
    endif()
endif(UNIX OR APPLE)

