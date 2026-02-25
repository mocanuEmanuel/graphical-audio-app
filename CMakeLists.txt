cmake_minimum_required(VERSION 3.20)
project(synth-err VERSION 1.0.0 LANGUAGES CXX)

# C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Raylib Config: Disable examples/games to speed up build
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/raylib)

# nlohmann/json (header-only, no build needed)
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(external/json)

#  .cpp files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Include Paths
target_include_directories(${PROJECT_NAME} PUBLIC 
    include 
    external/raylib/src
    external/json/include
)

if(WIN32)
    set(PLATFORM_LIBS winmm)
else()
    #crossplatform
    set(PLATFORM_LIBS m pthread dl rt X11)
endif()

# Link Raylib and Audio System
target_link_libraries(${PROJECT_NAME} PUBLIC 
    raylib 
    nlohmann_json::nlohmann_json
    ${PLATFORM_LIBS}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying assets to build directory..."
)