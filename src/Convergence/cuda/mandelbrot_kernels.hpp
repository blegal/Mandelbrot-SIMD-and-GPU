/*
 *  Copyright (c) 2026-... Bertrand LE GAL
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held
 *  liable for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute
 *  it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment
 *  in the product documentation would be appreciated but
 *  is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any
 *  source distribution.
 *
 */
#ifndef _mandelbrot_kernels_
#define _mandelbrot_kernels_
#if defined(_ENABLE_CUDA_)
/*
 *
 *
 *##############################################################################
 *
 *
 */
//
// Lanceurs des kernels CUDA (appelables depuis du code C++ standard).
// v_dat est un pointeur sur la memoire du GPU (cf. CudaEngine::reserve).
//
void mandelbrot_sp_launch(
          float* v_dat,     // le pointeur sur le tableau de sortie (GPU)
    const float  zoom,      // Le facteur de zoom
    const float  cornerX,   // la coordonnée X du coin superieur gauche
    const float  cornerY,   // la coordonnée Y du coin superieur gauche
    const int    width,     // la largeur de la fenetre
    const int    height,    // la hauteur de la fenetre
    const int    max_iters  // le nombre maximum d'itération
);

void mandelbrot_dp_launch(
          float*  v_dat,
    const double  zoom,
    const double  cornerX,
    const double  cornerY,
    const int     width,
    const int     height,
    const int     max_iters
);
#endif
#endif
