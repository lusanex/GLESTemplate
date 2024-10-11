cmake_minimum_required(VERSION 3.10)

project(App)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable debug symbols and disable optimization for gdb
set(CMAKE_C_FLAGS_DEBUG "-g -O0")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)


add_subdirectory(third_party/SDL)
add_subdirectory(third_party/cglm)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(third_party/assimp)


set(GLAD_SOURCE
    third_party/glad/src/glad.c)

# Source files for the project
set(PROJECT_SOURCES
    src/main.c   
)


# Add the executable target
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${GLAD_SOURCE})


target_include_directories( ${PROJECT_NAME} PRIVATE
    ${GOBJECT_INCLUDE_DIRS}
    third_party/SDL/include
    third_party/stb
    third_party/assimp
    third_party/glad/include
    third_party/cglm/include
    include  
)



target_link_libraries(${PROJECT_NAME} PRIVATE
            assimp
            SDL3::SDL3
            ${GOBJECT_LIBRARIES}
            cglm
            )


target_compile_options(${PROJECT_NAME} PRIVATE 
    ${GOBJECT_CFLAGS_OTHER}

    )


set(OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}-bin)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY}
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            ${OUTPUT_DIRECTORY}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${OUTPUT_DIRECTORY}/assets
)


