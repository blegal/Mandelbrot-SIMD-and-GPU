cmake_minimum_required(VERSION 3.7)

project(mandelbrot)

SET(CMAKE_BUILD_TYPE Release)

SET (CMAKE_CXX_STANDARD 17)

# APPLE INTEL FLAGS

#SET (CMAKE_C_COMPILER   /usr/local/opt/llvm/bin/clang)
#SET (CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)

#SET (CMAKE_CXX_FLAGS "")
#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -g0 -Wall -march=native -mtune=native -fopenmp")
#SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include")
#SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -I. -L/usr/local/lib")


# APPLE SILICON FLAGS

SET (CMAKE_C_COMPILER   /opt/homebrew/Cellar/llvm/15.0.5/bin/clang)
SET (CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/15.0.5/bin/clang++)

SET (CMAKE_CXX_FLAGS "")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -g0 -Wall -mcpu=native -mtune=native -fopenmp -I/opt/homebrew/opt/sfml/include")

# Linker libraries

SET (CMAKE_EXE_LINKER_FLAGS "-lm -fopenmp")

# Metal shader implementation for MacOS only

SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/opt/sfml/lib -L/opt/homebrew/lib")

# Metal shader implementation for MacOS only

SET (CMAKE_CXX_FLAGS        "${CMAKE_CXX_FLAGS}        -fno-objc-arc -stdlib=libc++")
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Metal")
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Foundation")
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework MetalKit")

# xcrun -sdk macosx metal -c src/vec_max.metal  -o bin/vec_max.air
# xcrun -sdk macosx metallib bin/vec_max.air    -o bin/vec_max.metallib

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
