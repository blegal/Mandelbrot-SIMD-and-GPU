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
#ifndef _MetalEngine_
#define _MetalEngine_
#if defined(_ENABLE_METAL_)
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
namespace MTL {
    class Device;
    class CommandQueue;
    class ComputePipelineState;
    class Buffer;
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
//
// Classe de base des moteurs Metal (metal-cpp, sans Objective-C) :
//  - le device et la file de commandes sont partages par tous les moteurs,
//  - le kernel est compile a la volee depuis le source embarque (mandelbrot.metal),
//  - le tampon de sortie est en memoire unifiee (StorageModeShared).
//
// Les classes filles fournissent le nom du kernel et ses parametres.
//
class MetalEngine : public Convergence {
public:
    MetalEngine(const std::string _name_, const std::string _kernel_, const bool _fastMath_);

    virtual ~MetalEngine();

    virtual bool is_valid();

protected:
    //
    // Lance le kernel sur une grille IMAGE_WIDTH x IMAGE_HEIGHT puis recopie
    // le resultat dans ptr. params est copie dans le buffer(1) du kernel.
    //
    void run(const void* params, const size_t length, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

private:
    bool init();

    std::string kernel;
    bool        fastMath;

    MTL::ComputePipelineState* _mPSO;
    MTL::Buffer*               _mBuffer;
    size_t                     _mBufferSize;

    static MTL::Device*        _mDevice;
    static MTL::CommandQueue*  _mCommandQueue;
};
#endif
#endif
