#/opt/homebrew/opt/llvm/bin/clang++ \
#    -std=c++17 -stdlib=libc++ -O2 \
#    -L/opt/homebrew/opt/libomp/lib -fopenmp \
#    -I./metal-cpp \
#    -fno-objc-arc -I./metal\
#    -framework Metal -framework Foundation -framework MetalKit \
#    -g src/main.cpp src/MetalAdder.cpp src/vadd/vec_add_x86.cpp  -o bin/benchmark.x 

#xcrun -sdk macosx metal -c ../src/kernel/metal/galaxeirb.metal -o MyLibrary.air
#xcrun -sdk macosx metallib MyLibrary.air                       -o default.metallib

xcrun -sdk macosx metal -c ../src/Convergence/mandelbrot/simple/metal/mono/mandelbrot_sp.metal -o mandelbrot_sp.air
xcrun -sdk macosx metallib                                                 mandelbrot_sp.air   -o mandelbrot_sp.metallib

#./bin/benchmark.x
