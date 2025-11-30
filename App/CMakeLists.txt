#
# App
#

set(SOURCES
    Source/Main.cpp
    Source/AppLayer.h
    Source/AppLayer.cpp
)

add_executable(Interloper)

target_sources(Interloper PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(Interloper Core)

target_include_directories(Interloper PRIVATE Source)

# Set working directory for VS debugger
set_target_properties(Interloper PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/App"
)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/App/Resources
    ${PROJECT_BINARY_DIR}/App/Resources
    COMMENT "Copying resources into binary directory")
