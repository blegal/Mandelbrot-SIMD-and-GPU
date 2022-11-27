/*
 *
 *
 *##############################################################################
 *
 *
 */
#include "SP_cuda.hpp"
#include <iostream>
/*
 *
 *
 *##############################################################################
 *
 *
 */
SP_cuda::SP_cuda() : Convergence("SP_CUDA")
{
    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "CUDA";

    // Initialisation of HOST dataset

    host_mBuffer  = nullptr;
    host_mZoom    = 0.f;
    host_mOffsetX = 0.f;
    host_mOffsetY = 0.f;
    host_mWidth   = 0;
    host_mHeight  = 0;
    host_mIters   = 0;

    // Initialisation of GPU dataset

    host_mBuffer = nullptr;
    gpu_mBuffer  = nullptr;
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
SP_cuda::SP_cuda(ColorMap* _colors, int _max_iters) : Convergence("SP_CUDA") {
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "none";
    modeOPENMP  = "disable";
    OTHER       = "CUDA";


    // Initialisation of HOST dataset

    host_mBuffer  = nullptr;
    host_mZoom    = 0.f;
    host_mOffsetX = 0.f;
    host_mOffsetY = 0.f;
    host_mWidth   = 0;
    host_mHeight  = 0;
    host_mIters   = 0;

    // Initialisation of GPU dataset

    host_mBuffer = nullptr;
    gpu_mBuffer  = nullptr;
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
SP_cuda::~SP_cuda( )
{
    if( gpu_mBuffer != nullptr)
        cudaFree( gpu_mBuffer );

    if( host_mBuffer != nullptr)
        free( host_mBuffer );
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
extern __global__  void mandelbrot_sp
(
          int*  v_dat,    // le pointeur sur le tableau de sortie
    const float zoom,     // Le facteur de zoom
    const float offsetX,  // la coordonnée X
    const float offsetY,  // la coordonnée Y
    const int   width,    // la largeur de la fenetre
    const int   height,   // la hauteur de la fenetre
    const int   max_iters // le nombre maximum d'itération
);

/*
 *
 *
 *##############################################################################
 *
 *
 */
void SP_cuda::updateImage(
        const long double _zoom,
        const long double _offsetX,
        const long double _offsetY,
        const int IMAGE_WIDTH,
        const int IMAGE_HEIGHT,
        float* ptr)
{
    cudaError_t err      = cudaSuccess;
    const int IMAGE_SIZE = IMAGE_WIDTH * IMAGE_HEIGHT;

    const long double corner_x = _offsetX - (IMAGE_WIDTH  / 2.0f) * _zoom;
    const long double corner_y = _offsetY - (IMAGE_HEIGHT / 2.0f) * _zoom;

    if( host_mBuffer == nullptr )
    {

        host_mBuffer = (int32_t*)malloc( IMAGE_SIZE * sizeof(int32_t) );
        if (host_mBuffer == nullptr) {
            fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }

        err = cudaMalloc((void **)&gpu_mBuffer, IMAGE_SIZE * sizeof(int32_t));
        if (err != cudaSuccess)
        {
            fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }
    }

    // End or parameter updates
    dim3 DimGrid (IMAGE_WIDTH, IMAGE_HEIGHT); // 5000 thread blocks
    dim3 DimBlock(16, 16);

    mandelbrot_sp<<<DimGrid, DimBlock>>>(
        gpu_mBuffer,
        _zoom,
        corner_x,
        corner_y,
        IMAGE_WIDTH,
        IMAGE_HEIGHT,
        max_iters
    );

    err = cudaGetLastError();
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to launch mandelbrot_sp kernel (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    err = cudaMemcpy(host_mBuffer, gpu_mBuffer, IMAGE_SIZE * sizeof(int32_t), cudaMemcpyDeviceToHost);
    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector gpu_c_pos from device to host (error code %s)!\n", cudaGetErrorString(err));
        fprintf(stderr, "issue happned in file %s at line %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }


    // On push back dans le buffer logiciel les resultats de calcul.

    for (int p = 0; p < IMAGE_SIZE; p += 1)
    {
        ptr[p] = host_mBuffer[p];
    }
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
bool SP_cuda::is_valid()
{
    return true;
}
