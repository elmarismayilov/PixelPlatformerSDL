cmake_minimum_required(VERSION 3.14)
project(PixelPlatformer VERSION 0.1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the target architectures (for both x86_64 and arm64)
set(CMAKE_OSX_ARCHITECTURES "arm64")

# Manually specify SDL2_image, SDL2_ttf, and SDL2_mixer paths
set(SDL2_IMAGE_INCLUDE_DIRS "/opt/homebrew/Cellar/sdl2_image/2.8.5/include/SDL2")
set(SDL2_IMAGE_LIBRARIES "/opt/homebrew/Cellar/sdl2_image/2.8.5/lib/libSDL2_image.dylib")

set(SDL2_TTF_INCLUDE_DIRS "/opt/homebrew/Cellar/sdl2_ttf/2.24.0/include/SDL2")
set(SDL2_TTF_LIBRARIES "/opt/homebrew/Cellar/sdl2_ttf/2.24.0/lib/libSDL2_ttf.dylib")

set(SDL2_MIXER_INCLUDE_DIRS "/opt/homebrew/Cellar/sdl2_mixer/2.8.1_1/include/SDL2")
set(SDL2_MIXER_LIBRARIES "/opt/homebrew/Cellar/sdl2_mixer/2.8.1_1/lib/libSDL2_mixer.dylib")

# Find packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Debugging: Show the paths and libraries found by CMake
message(STATUS "SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_image include dirs: ${SDL2_IMAGE_INCLUDE_DIRS}")
message(STATUS "SDL2_ttf include dirs: ${SDL2_TTF_INCLUDE_DIRS}")
message(STATUS "SDL2_mixer include dirs: ${SDL2_MIXER_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries: ${SDL2_LIBRARIES}")
message(STATUS "SDL2_image libraries: ${SDL2_IMAGE_LIBRARIES}")
message(STATUS "SDL2_ttf libraries: ${SDL2_TTF_LIBRARIES}")
message(STATUS "SDL2_mixer libraries: ${SDL2_MIXER_LIBRARIES}")

# Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
)

# Collect source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES}
        include/Game.h
        src/Game.cpp
        include/Player.h
        src/Player.cpp
        include/World.h
        src/World.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
)

# Copy assets
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
