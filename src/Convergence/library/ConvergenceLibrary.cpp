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
#include "ConvergenceLibrary.hpp"

////////    MANDELBROT FIXED POINT      ////////

#include "Convergence/mandelbrot/quad/x86/mono/QP_x86.hpp"
#include "Convergence/mandelbrot/quad/x86/multi/QP_x86_OMP.hpp"


////////    MANDELBROT FLOAT POINT      ////////

#include "Convergence/mandelbrot/simple/x86/mono/SP_x86.hpp"
#include "Convergence/mandelbrot/simple/x86/multi/SP_x86_OMP.hpp"

#include "Convergence/mandelbrot/simple/sse4/mono/SP_SSE4.hpp"
#include "Convergence/mandelbrot/simple/sse4/mono/SP_SSE4_vc.hpp"

#include "Convergence/mandelbrot/simple/sse4/multi/SP_SSE4_OMP.hpp"
#include "Convergence/mandelbrot/simple/sse4/multi/SP_SSE4_OMP_vc.hpp"

#include "Convergence/mandelbrot/simple/neon/mono/SP_NEON.hpp"
#include "Convergence/mandelbrot/simple/neon/multi/SP_NEON_OMP.hpp"

#include "Convergence/mandelbrot/simple/avx2/mono/SP_AVX2.hpp"
#include "Convergence/mandelbrot/simple/avx2/mono/SP_AVX2_u2.hpp"

#include "Convergence/mandelbrot/simple/avx2/multi/SP_AVX2_OMP.hpp"
#include "Convergence/mandelbrot/simple/avx2/multi/SP_AVX2_OMP_u2.hpp"

#include "Convergence/mandelbrot/simple/avx512/mono/SP_AVX512.hpp"
#include "Convergence/mandelbrot/simple/avx512/multi/SP_AVX512_OMP.hpp"


////////    MANDELBROT DOUBLE POINT      ////////

#include "Convergence/mandelbrot/double/x86/mono/DP_x86.hpp"
#include "Convergence/mandelbrot/double/x86/multi/DP_x86_OMP.hpp"

#include "Convergence/mandelbrot/double/sse4/mono/DP_SSE4.hpp"
#include "Convergence/mandelbrot/double/sse4/multi/DP_SSE4_OMP.hpp"

#include "Convergence/mandelbrot/double/neon/mono/DP_NEON.hpp"
#include "Convergence/mandelbrot/double/neon/multi/DP_NEON_OMP.hpp"

#include "Convergence/mandelbrot/double/avx2/mono/normal/DP_AVX2.hpp"
#include "Convergence/mandelbrot/double/avx2/multi/normal/DP_AVX2_OMP.hpp"

#include "Convergence/mandelbrot/double/avx2/mono/unroll_x2/DP_AVX2_u2.hpp"
#include "Convergence/mandelbrot/double/avx2/multi/unroll_x2/DP_AVX2_OMP_u2.hpp"

#include "Convergence/mandelbrot/double/avx2/mono/unroll_x4/DP_AVX2_u4.hpp"
#include "Convergence/mandelbrot/double/avx2/multi/unroll_x4/DP_AVX2_OMP_u4.hpp"

#include "Convergence/mandelbrot/double/avx512/mono/DP_AVX512.hpp"
#include "Convergence/mandelbrot/double/avx512/multi/DP_AVX512_OMP.hpp"

////////      GPU VERSIONS (FLOAT and double)      ////////

#include "Convergence/mandelbrot/simple/metal/mono/SP_metal.hpp"

#include "Convergence/mandelbrot/simple/cuda/mono/SP_cuda.hpp"
#include "Convergence/mandelbrot/double/cuda/mono/DP_cuda.hpp"

////////      FLOAT POINT      ////////

#include "Convergence/mandelbrot/simple/x86/mono/SP_x86.hpp"
#include "Convergence/mandelbrot/simple/x86/multi/SP_x86_OMP.hpp"

#include "Convergence/mandelbrot/simple/sse4/mono/SP_SSE4.hpp"
#include "Convergence/mandelbrot/simple/sse4/mono/SP_SSE4_vc.hpp"

#include "Convergence/mandelbrot/simple/sse4/multi/SP_SSE4_OMP.hpp"
#include "Convergence/mandelbrot/simple/sse4/multi/SP_SSE4_OMP_vc.hpp"

#include "Convergence/mandelbrot/simple/neon/mono/SP_NEON.hpp"
#include "Convergence/mandelbrot/simple/neon/multi/SP_NEON_OMP.hpp"

#include "Convergence/mandelbrot/simple/avx2/mono/SP_AVX2.hpp"
#include "Convergence/mandelbrot/simple/avx2/mono/SP_AVX2_u2.hpp"

#include "Convergence/mandelbrot/simple/avx2/multi/SP_AVX2_OMP.hpp"
#include "Convergence/mandelbrot/simple/avx2/multi/SP_AVX2_OMP_u2.hpp"

#include "Convergence/mandelbrot/simple/avx512/mono/SP_AVX512.hpp"
#include "Convergence/mandelbrot/simple/avx512/multi/SP_AVX512_OMP.hpp"

////////      FPGA <=> ETHERNET      ////////

#include "Convergence/mandelbrot/simple/FPGA/mono/SP_FPGA.hpp"

////////      FIXED POINT      ////////

#include "Convergence/mandelbrot/fixed/x86/mono/FP_Q16_11_x86.hpp"
#include "Convergence/mandelbrot/fixed/x86/mono/FP_Q18_14_x86.hpp"
#include "Convergence/mandelbrot/fixed/x86/mono/FP_Q32_27_x86.hpp"

#include "Convergence/mandelbrot/fixed/x86/mono/Convergence_fp_x86.hpp"
#include "Convergence/mandelbrot/fixed/x86/multi/Convergence_fp_x86_omp.hpp"

#include "Convergence/mandelbrot/fixed/sse4/Convergence_fp_x86_omp_SSE2.hpp"


////    ////    ////    ////    ////    ////    ////
#include "Convergence/julia/simple/x86/julia-mono/JU_SP_x86.hpp"
#include "Convergence/julia/simple/avx512/mono-julia/JU_SP_AVX512.hpp"
#include "Convergence/julia/simple/avx512/multi-julia/JU_SP_AVX512_OMP.hpp"

////////      FLOAT POINT      ////////

#include "Convergence/julia/DP_x86_BS/DP_x86_BS.hpp"
#include "Convergence/julia/double/DP_x86_J2/DP_x86_J2.hpp"
#include "Convergence/julia/double/DP_x86_J3/DP_x86_J3.hpp"
#include "Convergence/julia/double/DP_x86_J4/DP_x86_J4.hpp"
#include "Convergence/julia/double/DP_x86_J5/DP_x86_J5.hpp"
#include "Convergence/julia/double/DP_x86_J6/DP_x86_J6.hpp"
#include "Convergence/julia/double/DP_x86_J7/DP_x86_J7.hpp"
#include "Convergence/julia/double/DP_x86_J8/DP_x86_J8.hpp"
#include "Convergence/julia/double/DP_x86_J9/DP_x86_J9.hpp"
#include "Convergence/julia/double/DP_x86_J10/DP_x86_J10.hpp"

////    ////    ////    ////    ////    ////    ////

ConvergenceLibrary::ConvergenceLibrary()
{

    //
    //
    //

    list.push_back( new SP_x86         (nullptr, 255) );
    list.push_back( new SP_x86_OMP     (nullptr, 255) );

#ifdef __SSE4_2__
    list.push_back( new SP_SSE4        (nullptr, 255) );
    list.push_back( new SP_SSE4_vc     (nullptr, 255) );

    list.push_back( new SP_SSE4_OMP    (nullptr, 255) );
    list.push_back( new SP_SSE4_OMP_vc (nullptr, 255) );
#endif

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
    list.push_back( new SP_NEON        (nullptr, 255) );
    list.push_back( new SP_NEON_OMP    (nullptr, 255) );
#endif

#ifdef __AVX2__
    list.push_back( new SP_AVX2        (nullptr, 255) );
    list.push_back( new SP_AVX2_u2     (nullptr, 255) );

    list.push_back( new SP_AVX2_OMP    (nullptr, 255) );
    list.push_back( new SP_AVX2_OMP_u2 (nullptr, 255) );
#endif

    list.push_back( new SP_AVX512      (nullptr, 255) );
    list.push_back( new SP_AVX512_OMP  (nullptr, 255) );

    //
    //////////////////////////////////////////////////////////////////////
    //

#if defined(__APPLE__)
    list.push_back( new SP_metal(nullptr, 255) );
#endif

#if defined(_ENABLE_CUDA_)
    list.push_back( new SP_cuda(nullptr, 255) );
#endif


    //
    //////////////////////////////////////////////////////////////////////
    //

    list.push_back( new DP_x86        (nullptr, 255) );
    list.push_back( new DP_x86_OMP    (nullptr, 255) );

#ifdef __SSE4_2__
    list.push_back( new DP_SSE4       (nullptr, 255) );
    list.push_back( new DP_SSE4_OMP   (nullptr, 255) );
#endif

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
    list.push_back( new DP_NEON        (nullptr, 255) );
    list.push_back( new DP_NEON_OMP    (nullptr, 255) );
#endif

#ifdef __AVX2__
    list.push_back( new DP_AVX2       (nullptr, 255) );
    list.push_back( new DP_AVX2_OMP   (nullptr, 255) );

    list.push_back( new DP_AVX2_u2    (nullptr, 255) );
    list.push_back( new DP_AVX2_OMP_u2(nullptr, 255) );

    list.push_back( new DP_AVX2_u4    (nullptr, 255) );
    list.push_back( new DP_AVX2_OMP_u4(nullptr, 255) );
#endif

    list.push_back( new DP_AVX512      (nullptr, 255) );
    list.push_back( new DP_AVX512_OMP  (nullptr, 255) );

#if defined(_ENABLE_CUDA_)
    list.push_back( new DP_cuda(nullptr, 255) );
#endif

    //
    //////////////////////////////////////////////////////////////////////
    //

//    list.push_back( new SP_FPGA        (nullptr, 255) );

    //
    //////////////////////////////////////////////////////////////////////
    //

    list.push_back( new QP_x86                (nullptr, 255) );
    list.push_back( new QP_x86_OMP            (nullptr, 255) );

    //
    //////////////////////////////////////////////////////////////////////
    //

    list.push_back( new FP_Q16_11_x86               (nullptr, 255) );
    list.push_back( new FP_Q18_14_x86               (nullptr, 255) );
    list.push_back( new FP_Q32_27_x86               (nullptr, 255) );
    list.push_back( new Convergence_fp_x86          (nullptr, 255) );
    list.push_back( new Convergence_fp_x86_omp      (nullptr, 255) );
//  list.push_back( new Convergence_fp_x86_omp_SSE2 (nullptr, 255) );

    //
    //////////////////////////////////////////////////////////////////////
    //

    list.push_back( new JU_SP_x86       (nullptr, 255) );

    list.push_back( new JU_SP_AVX512    (nullptr, 255) );
    list.push_back( new JU_SP_AVX512_OMP(nullptr, 255) );

    list.push_back( new DP_x86_BS (nullptr, 255) );
    list.push_back( new DP_x86_J2 (nullptr, 255) );
    list.push_back( new DP_x86_J3 (nullptr, 255) );
    list.push_back( new DP_x86_J4 (nullptr, 255) );
    list.push_back( new DP_x86_J5 (nullptr, 255) );
    list.push_back( new DP_x86_J6 (nullptr, 255) );
    list.push_back( new DP_x86_J7 (nullptr, 255) );
    list.push_back( new DP_x86_J8 (nullptr, 255) );
    list.push_back( new DP_x86_J9 (nullptr, 255) );
    list.push_back( new DP_x86_J10(nullptr, 255) );

    //
    //////////////////////////////////////////////////////////////////////
    //

    std::cout << "INSTANCIATED MODULES:" << std::endl;
    std::cout << "--------------------"  << std::endl;

    listAllModules();

    std::cout << "UNLOADING USELESS MODULES:" << std::endl;
    std::cout << "-------------------------"  << std::endl;

    for(int i=0; i<list.size(); i++){
        const bool enable = list[i]->is_valid();
        if( enable == false )
        {
            cout << list[i]->toString() << std::endl;
            Convergence* c = list[i];
            list.erase( list.begin() + i );
            delete c;
            i -= 1;
        }
    }

    std::cout << "FINALLY AVAILABLE MODULES:" << std::endl;
    std::cout << "-------------------------"  << std::endl;

    listAllModules();

    std::cout << "-------------------------"  << std::endl;

    //
    //
    //
    for(int i=0; i<list.size(); i++){
        dico[ list[i]->name() ] = list[i];
        indx[ list[i]->name() ] =      i ;
    }

    counter = 0;

}

ConvergenceLibrary::~ConvergenceLibrary(){

    for(int i=0; i<list.size(); i++)
        delete list[i];

}

Convergence* ConvergenceLibrary::get(std::string name){
    counter = indx[name];
    return get();
}

Convergence* ConvergenceLibrary::get(int num){
    counter = num;
    return get();
}

int32_t ConvergenceLibrary::size( )
{
    return list.size();
}

Convergence* ConvergenceLibrary::get(){
//        printf("GET %d : (%p) name = %s \n", counter, list[counter], list[counter]->name().c_str());
    //printf("Convergence :: get (%d) : (%p) name = %s \n", counter, list[counter], list[counter]->name().c_str());
    std::cout << "(II) Switching to " << list[counter]->toString() << std::endl;
    return list[counter];
}

void ConvergenceLibrary::next(){
    counter = (counter + 1) % list.size();
}

void ConvergenceLibrary::previous(){
    counter = (counter + list.size() - 1) % list.size();
}

void ConvergenceLibrary::listAllModules()
{
    for(int32_t i = 0; i < list.size(); i+= 1)
    {
        printf("%2d : ", i);
        cout << list[i]->toString() << std::endl;
    }
}
