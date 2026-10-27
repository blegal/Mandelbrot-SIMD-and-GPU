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
#ifndef _CudaEngine_
#define _CudaEngine_
#if defined(_ENABLE_CUDA_)
/*
 *
 *
 *##############################################################################
 *
 *
 */
#include "Convergence/Convergence.hpp"
/*
 *
 *
 *##############################################################################
 *
 *
 */
//
// Classe de base des moteurs CUDA : gestion du tampon de sortie sur le GPU
// (re-alloue si la taille de l'image change) et rapatriement des resultats.
// Les kernels sont dans mandelbrot_kernels.cu.
//
class CudaEngine : public Convergence {
public:
    CudaEngine(const std::string _name_);

    virtual ~CudaEngine();

    virtual bool is_valid();

protected:
    float* reserve (const size_t IMAGE_SIZE);
    void   download(float* ptr, const size_t IMAGE_SIZE);

private:
    float* gpu_mBuffer;
    size_t gpu_mSize;
};
#endif
#endif
