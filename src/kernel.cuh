#ifndef __KERNEL_CUH__
#define __KERNEL_CUH__

void compute_cuda_d(int nthreads, double zoom, double offsetX, double offsetY, unsigned int max_iters,
  int IMAGE_WIDTH, int IMAGE_HEIGHT, unsigned short *device_value);

void compute_cuda_d2(int nthreads, double zoom, double offsetX, double offsetY, unsigned int max_iters,
  int IMAGE_WIDTH, int IMAGE_HEIGHT, unsigned short *device_value);

void compute_cuda_d2_wp(int nthreads, double zoom, double offsetX, double offsetY, unsigned int max_iters,
  int IMAGE_WIDTH, int IMAGE_HEIGHT, unsigned short *device_value);

void compute_cuda_s(int nthreads, float zoom, float offsetX, float offsetY, unsigned int max_iters,
  int IMAGE_WIDTH, int IMAGE_HEIGHT, unsigned short *device_value);

#endif
