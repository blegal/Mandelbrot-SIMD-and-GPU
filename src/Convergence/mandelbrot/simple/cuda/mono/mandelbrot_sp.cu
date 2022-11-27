
__device__ unsigned short process_cuda_d(const double startReal, const double startImag, unsigned short max_iters)  {
  double zReal = startReal;
  double zImag = startImag;

  for (unsigned short counter = 0; counter < max_iters; counter++) {
    double r2 = zReal * zReal;
    double i2 = zImag * zImag;
    zImag = 2.0f * zReal * zImag + startImag;
    zReal = r2 - i2 + startReal;
    if ( (r2 + i2) > 4.0f) {
      return counter;
    }
  }
  return max_iters - 1;
}


__global__ void kernel_compute_cuda_d(double zoom, double offsetX, double offsetY, unsigned int max_iters,
  int IMAGE_WIDTH, int IMAGE_HEIGHT, unsigned short *device_value) {

	int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  double imag;
  double real;

  if ( (x<IMAGE_WIDTH) && (y<IMAGE_HEIGHT) ) {
    imag = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (y * zoom);
    real = (offsetX - IMAGE_WIDTH / 2.0f * zoom)+((x)*zoom);
    device_value[y*IMAGE_WIDTH +x] = process_cuda_d(real, imag, max_iters);
	}
}


void compute_cuda_d(int nthreads, double zoom, double offsetX, double offsetY, unsigned int max_iters,
  int IMAGE_WIDTH, int IMAGE_HEIGHT, unsigned short *device_value) {

  int numBlocksX = ( IMAGE_WIDTH + ( nthreads - 1 ) ) / nthreads;
  int numBlocksY = ( IMAGE_HEIGHT + ( nthreads - 1 ) ) / nthreads;

  dim3 DimBlock(nthreads, nthreads, 1);
  dim3 DimGrid(numBlocksX, numBlocksY);

  kernel_compute_cuda_d<<<DimGrid, DimBlock>>>( zoom, offsetX, offsetY, max_iters,
    IMAGE_WIDTH, IMAGE_HEIGHT, device_value);
}



/******************************/
/*         CUDA DOUBLE2       */
/******************************/

__device__ unsigned short process_cuda_d2(double2 startValue, unsigned short max_iters)  {

  double2 startZ = startValue;

  double2 z1;
  z1.x = startZ.x;
  z1.y = startZ.y;

  for (unsigned short counter = 0; counter < max_iters; counter++) {
    double2 z2;
    z2.x = z1.x * z1.x;
    z2.y = z1.y * z1.y;

    z1.y = 2.0f * z1.x * z1.y + startZ.y;
    z1.x = z2.x - z2.y + startZ.x;
    if ( (z2.x + z2.y) > 4.0f) {
      return counter;
    }
  }
  return max_iters - 1;
}

__global__ void kernel_compute_cuda_d2(double zoom, double offsetX, double offsetY, unsigned int max_iters,
  int IMAGE_WIDTH, int IMAGE_HEIGHT, unsigned short *device_value) {

	int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  double2 z;

  if ( (x<IMAGE_WIDTH) && (y<IMAGE_HEIGHT) ) {
    z.y = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (y * zoom);
    z.x = (offsetX - IMAGE_WIDTH / 2.0f * zoom)+((x)*zoom);
    device_value[y*IMAGE_WIDTH + x] = process_cuda_d2(z, max_iters);
	}
}


void compute_cuda_d2(int nthreads, double zoom, double offsetX, double offsetY, unsigned int max_iters,
  int IMAGE_WIDTH, int IMAGE_HEIGHT, unsigned short *device_value) {

  int numBlocksX = ( IMAGE_WIDTH + ( nthreads - 1 ) ) / nthreads;
  int numBlocksY = ( IMAGE_HEIGHT + ( nthreads - 1 ) ) / nthreads;

  dim3 DimBlock(nthreads, nthreads, 1);
  dim3 DimGrid(numBlocksX, numBlocksY);

  kernel_compute_cuda_d2<<<DimGrid, DimBlock>>>( zoom, offsetX, offsetY, max_iters,
    IMAGE_WIDTH, IMAGE_HEIGHT, device_value);

}


/******************************/
/*CUDA DOUBLE WITHOUT PROCESS */
/******************************/

__global__ void kernel_compute_cuda_d2_wp(double zoom, double offsetX, double offsetY, unsigned int max_iters,
  int IMAGE_WIDTH, int IMAGE_HEIGHT, unsigned short *device_value) {

	int i = blockIdx.x * blockDim.x + threadIdx.x;
  int j = blockIdx.y * blockDim.y + threadIdx.y;

  double2 z;
  double2 z1;
  double2 z2;

  if ( (i<IMAGE_WIDTH) && (j<IMAGE_HEIGHT) ) {
    z.y = offsetY - IMAGE_HEIGHT / 2.0f * zoom + (j * zoom);
    z.x = (offsetX - IMAGE_WIDTH / 2.0f * zoom)+ (i * zoom);

    z1.x = z.x;
    z1.y = z.y;

    device_value[j*IMAGE_WIDTH + i] = max_iters - 1;

    for (unsigned short counter = 0; counter < max_iters; counter++) {
      z2.x = z1.x * z1.x;
      z2.y = z1.y * z1.y;

      z1.y = 2.0f * z1.x * z1.y + z.y;
      z1.x = z2.x - z2.y + z.x;
      if ( (z2.x + z2.y) > 4.0f) {
        device_value[j*IMAGE_WIDTH + i] = counter;
        break;
      }
    }
	}
}

void compute_cuda_d2_wp (int nthreads, double zoom, double offsetX, double offsetY, unsigned int max_iters,
  int IMAGE_WIDTH, int IMAGE_HEIGHT, unsigned short *device_value) {

  int numBlocksX = ( IMAGE_WIDTH + ( nthreads - 1 ) ) / nthreads;
  int numBlocksY = ( IMAGE_HEIGHT + ( nthreads - 1 ) ) / nthreads;

  dim3 DimBlock(nthreads, nthreads, 1);
  dim3 DimGrid(numBlocksX, numBlocksY);

  kernel_compute_cuda_d2_wp<<<DimGrid, DimBlock>>>( zoom, offsetX, offsetY, max_iters,
    IMAGE_WIDTH, IMAGE_HEIGHT, device_value);
}



/******************************/
/*        CUDA SIMPLE         */
/******************************/
__device__ unsigned short process_cuda_s(const float startReal, const float startImag, unsigned short max_iters)  {
  float zReal = startReal;
  float zImag = startImag;

  for (unsigned short counter = 0; counter < max_iters; counter++) {
    float r2 = zReal * zReal;
    float i2 = zImag * zImag;
    zImag = 2.0f * zReal * zImag + startImag;
    zReal = r2 - i2 + startReal;
    if ( (r2 + i2) > 4.0f) {
      return counter;
    }
  }
  return max_iters - 1;
}

__global__ void mandelbrot_sp
    (
              int*  v_dat,    // le pointeur sur le tableau de sortie
        const float zoom,     // Le facteur de zoom
        const float offsetX,  // la coordonnée X
        const float offsetY,  // la coordonnée Y
        const int   width,    // la largeur de la fenetre
        const int   height,   // la hauteur de la fenetre
        const int   max_iters // le nombre maximum d'itération
    )
{

    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    if ( (x < width) && (y < height) )
    {
        const float imag  = offsetY  + (y * zoom);
        const float real  = offsetX + (x * zoom);
        v_dat[y*width +x] = process_cuda_s(real, imag, max_iters);
	}
}
