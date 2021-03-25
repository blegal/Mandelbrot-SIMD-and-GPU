cmake_minimum_required(VERSION 3.7)

project(mandelbrot)

SET(CMAKE_BUILD_TYPE Release)

SET (CMAKE_CXX_STANDARD 17)

# APPLE INTEL FLAGS

SET (CMAKE_C_COMPILER   /usr/local/opt/llvm/bin/clang)
SET (CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)

SET (CMAKE_CXX_FLAGS "")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g0 -Wall -march=native -mtune=native -fopenmp")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include")
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -I. -L/usr/local/lib")


# APPLE SILICON FLAGS

#SET (CMAKE_C_COMPILER   /opt/homebrew/Cellar/llvm/11.1.0/bin/clang)
#SET (CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/11.1.0/bin/clang++)

#SET (CMAKE_CXX_FLAGS "")
#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g0 -Wall -mtune=native -fopenmp -I/opt/homebrew/opt/sfml/include")
#SET (CMAKE_EXE_LINKER_FLAGS "-I. -L/opt/homebrew/opt/sfml/lib -L/opt/homebrew/lib -lm -fopenmp -lsfml-graphics -lsfml-window -lsfml-system")

# Generate the source files list
file (GLOB_RECURSE source_files src/*)

# Add includes
include_directories (src)

# Object
add_library (dec-obj OBJECT ${source_files})

add_executable(mandelbrot $<TARGET_OBJECTS:dec-obj>)

target_link_libraries(mandelbrot sfml-graphics)
target_link_libraries(mandelbrot sfml-window)
target_link_libraries(mandelbrot sfml-system)
