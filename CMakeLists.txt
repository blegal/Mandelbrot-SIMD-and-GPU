cmake_minimum_required(VERSION 3.16)

##################################################################
#
# Mandelbrot - SIMD and GPU
#
# Configuration portable : Linux, macOS (Intel / Apple Silicon), Windows.
#
#   cmake -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build -j
#
# Options (-D<OPTION>=ON/OFF) :
#
#   MANDELBROT_ENABLE_GUI     interface graphique GLFW + OpenGL       (ON)
#   MANDELBROT_FETCH_GLFW     telecharge GLFW s'il n'est pas installe (ON)
#   MANDELBROT_ENABLE_OPENMP  moteurs multi-coeurs (OpenMP)           (ON)
#   MANDELBROT_ENABLE_METAL   moteurs GPU Metal (AUTO, ON, OFF)       (AUTO)
#   MANDELBROT_ENABLE_CUDA    moteurs GPU CUDA  (AUTO, ON, OFF)       (AUTO)
#   MANDELBROT_NATIVE         optimise pour le CPU de la machine      (ON)
#
##################################################################

project(mandelbrot LANGUAGES C CXX)

option(MANDELBROT_ENABLE_GUI    "Build the GLFW/OpenGL graphical interface"   ON)
option(MANDELBROT_FETCH_GLFW    "Download GLFW when it is not installed"      ON)
option(MANDELBROT_ENABLE_OPENMP "Enable the multi-threaded (OpenMP) engines"  ON)
option(MANDELBROT_NATIVE        "Optimize for the host CPU (-march=native)"   ON)
set   (MANDELBROT_ENABLE_METAL "AUTO" CACHE STRING "Enable the Metal GPU engines (AUTO, ON or OFF)")
set_property(CACHE MANDELBROT_ENABLE_METAL PROPERTY STRINGS AUTO ON OFF)
set   (MANDELBROT_ENABLE_CUDA "AUTO" CACHE STRING "Enable the CUDA GPU engines (AUTO, ON or OFF)")
set_property(CACHE MANDELBROT_ENABLE_CUDA PROPERTY STRINGS AUTO ON OFF)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif ()

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

##################################################################
#
# Generate the source files list
#

file (GLOB_RECURSE source_files CONFIGURE_DEPENDS src/*.cpp)

list (FILTER source_files EXCLUDE REGEX "/src/Display/")
list (FILTER source_files EXCLUDE REGEX "/metal/")

if (WIN32)
    # Le moteur FPGA repose sur les sockets POSIX
    list (FILTER source_files EXCLUDE REGEX "/FPGA/")
endif ()

add_executable(mandelbrot ${source_files})

target_include_directories(mandelbrot PRIVATE src)

##################################################################
#
# Compiler flags
#

include(CheckCXXCompilerFlag)

if (MSVC)
    target_compile_options(mandelbrot PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/O2 /fp:fast /W3 /permissive->)
    target_compile_definitions(mandelbrot PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
    if (MANDELBROT_NATIVE)
        check_cxx_compiler_flag("/arch:AVX2" HAS_ARCH_AVX2)
        if (HAS_ARCH_AVX2)
            target_compile_options(mandelbrot PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/arch:AVX2>)
        endif ()
    endif ()
else ()
    target_compile_options(mandelbrot PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-O3 -ffast-math -Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-sign-compare>)
    if (MANDELBROT_NATIVE)
        # -mcpu=native sur ARM (Apple Silicon, Jetson, Raspberry Pi, ...), -march=native sur x86
        if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64|aarch64|ARM64)$")
            check_cxx_compiler_flag("-mcpu=native" HAS_MCPU_NATIVE)
            if (HAS_MCPU_NATIVE)
                target_compile_options(mandelbrot PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-mcpu=native>)
            endif ()
        else ()
            check_cxx_compiler_flag("-march=native" HAS_MARCH_NATIVE)
            if (HAS_MARCH_NATIVE)
                target_compile_options(mandelbrot PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-march=native -mtune=native>)
            endif ()
        endif ()
    endif ()
endif ()

##################################################################
#
# OpenMP (sur macOS : brew install libomp)
#

if (MANDELBROT_ENABLE_OPENMP)
    if (APPLE AND NOT OpenMP_ROOT AND NOT DEFINED ENV{OpenMP_ROOT})
        find_program(BREW_EXECUTABLE brew)
        if (BREW_EXECUTABLE)
            execute_process(COMMAND ${BREW_EXECUTABLE} --prefix libomp
                            OUTPUT_VARIABLE LIBOMP_PREFIX
                            OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
            if (LIBOMP_PREFIX AND EXISTS "${LIBOMP_PREFIX}")
                set(OpenMP_ROOT "${LIBOMP_PREFIX}")
            endif ()
        endif ()
    endif ()

    find_package(OpenMP COMPONENTS CXX)
    if (OpenMP_CXX_FOUND)
        target_link_libraries(mandelbrot PRIVATE OpenMP::OpenMP_CXX)
    else ()
        message(WARNING "OpenMP not found : the *_OMP engines will run on a single core "
                        "(on macOS : brew install libomp)")
    endif ()
endif ()

##################################################################
#
# Graphical interface : GLFW + OpenGL
#

if (MANDELBROT_ENABLE_GUI)
    set(OpenGL_GL_PREFERENCE GLVND)
    find_package(OpenGL REQUIRED)

    find_package(glfw3 3.3 CONFIG QUIET)
    if (NOT glfw3_FOUND)
        if (MANDELBROT_FETCH_GLFW)
            message(STATUS "GLFW not found : downloading it (MANDELBROT_FETCH_GLFW=ON)")
            include(FetchContent)
            set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
            set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
            set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
            set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
            FetchContent_Declare(glfw
                GIT_REPOSITORY https://github.com/glfw/glfw.git
                GIT_TAG        3.4
                GIT_SHALLOW    TRUE)
            FetchContent_MakeAvailable(glfw)
        else ()
            message(FATAL_ERROR "GLFW not found : install it, or set MANDELBROT_FETCH_GLFW=ON "
                                "or MANDELBROT_ENABLE_GUI=OFF (headless build)")
        endif ()
    endif ()

    file (GLOB display_files CONFIGURE_DEPENDS src/Display/*.cpp)
    target_sources            (mandelbrot PRIVATE ${display_files})
    target_link_libraries     (mandelbrot PRIVATE glfw OpenGL::GL)
    target_compile_definitions(mandelbrot PRIVATE _ENABLE_GUI_)
endif ()

##################################################################
#
# Metal (macOS) : le kernel est embarque dans l'executable puis compile a la
# volee par le driver => pas besoin de la "Metal Toolchain" d'Xcode.
#

# AUTO : active sur macOS avec Clang, ignore silencieusement ailleurs (Linux, Windows)

set(METAL_AVAILABLE OFF)
if (APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(METAL_AVAILABLE ON)
endif ()

if (MANDELBROT_ENABLE_METAL STREQUAL "ON" AND NOT METAL_AVAILABLE)
    message(FATAL_ERROR "MANDELBROT_ENABLE_METAL=ON but Metal requires macOS and Clang")
endif ()

if (NOT MANDELBROT_ENABLE_METAL STREQUAL "OFF")
    if (NOT METAL_AVAILABLE)
        message(STATUS "Metal engines : disabled (macOS + Clang only)")
    else ()
        set(METAL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Convergence/metal/mandelbrot.metal)
        set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${METAL_SOURCE})
        file(READ ${METAL_SOURCE} METAL_CODE)
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/generated/mandelbrot_metal_source.hpp
             "// Generated by CMake from src/Convergence/metal/mandelbrot.metal : do not edit\n"
             "static const char* MANDELBROT_METAL_SOURCE = R\"__METAL__(\n${METAL_CODE}\n)__METAL__\";\n")

        file (GLOB_RECURSE metal_files CONFIGURE_DEPENDS src/*.cpp)
        list (FILTER metal_files INCLUDE REGEX "/metal/")
        target_sources            (mandelbrot PRIVATE ${metal_files})
        target_include_directories(mandelbrot PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)
        target_compile_definitions(mandelbrot PRIVATE _ENABLE_METAL_)
        target_link_libraries     (mandelbrot PRIVATE "-framework Metal" "-framework Foundation" "-framework QuartzCore")
        # metal-cpp manipule INFINITY : incompatible avec -ffast-math
        set_source_files_properties(src/Convergence/metal/MetalEngine.cpp PROPERTIES COMPILE_OPTIONS "-fno-finite-math-only")
        message(STATUS "Metal engines : enabled")
    endif ()
endif ()

##################################################################
#
# CUDA (NVIDIA GPUs : PC, Jetson Orin, ...)
#

if (NOT MANDELBROT_ENABLE_CUDA STREQUAL "OFF")
    include(CheckLanguage)
    check_language(CUDA)
    if (CMAKE_CUDA_COMPILER)
        if (NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
            if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
                set(CMAKE_CUDA_ARCHITECTURES native)
            else ()
                set(CMAKE_CUDA_ARCHITECTURES 75 86 87)
            endif ()
        endif ()
        enable_language(CUDA)
        find_package(CUDAToolkit REQUIRED)

        set(CMAKE_CUDA_STANDARD          17)
        set(CMAKE_CUDA_STANDARD_REQUIRED ON)

        file (GLOB cuda_files CONFIGURE_DEPENDS src/Convergence/cuda/*.cu)
        target_sources            (mandelbrot PRIVATE ${cuda_files})
        target_compile_definitions(mandelbrot PRIVATE _ENABLE_CUDA_)
        target_compile_options    (mandelbrot PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-O3 --use_fast_math>)
        target_link_libraries     (mandelbrot PRIVATE CUDA::cudart)
        message(STATUS "CUDA engines : enabled (architectures : ${CMAKE_CUDA_ARCHITECTURES})")
    elseif (MANDELBROT_ENABLE_CUDA STREQUAL "ON")
        message(FATAL_ERROR "MANDELBROT_ENABLE_CUDA=ON but no CUDA compiler was found")
    endif ()
endif ()

##################################################################

install(TARGETS mandelbrot RUNTIME DESTINATION bin)
