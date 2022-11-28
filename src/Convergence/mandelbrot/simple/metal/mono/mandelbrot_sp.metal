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
/*
See LICENSE-original.txt for this sample’s licensing information.

Abstract:
A shader that adds two arrays of floats.
*/


#include <metal_stdlib>
using namespace metal;
/*
 *
 *
 *##############################################################################
 *
 *
 */
kernel void mandelbrot_sp(
          device uint*  v_dat, // le pointeur sur le tableau de sortie
    const device float* zoom, // Le facteur de zoom
    const device float* offsetX, // la coordonnée X
    const device float* offsetY, // la coordonnée Y
    const device uint*  width, // la largeur de la fenetre
    const device uint*  height, // la hauteur de la fenetre
    const device uint*  max_iters, // le nombre maximum d'itérations
    uint2 position [[thread_position_in_grid]]   // La position du calcul à réaliser
)
{
    //
    // Si jamais notre position dans la grille déborde
    //
    const uint  v_width     = (*width);
    const uint  v_height    = (*height);
    const float v_zoom      = (*zoom);
    const float v_offsetX   = (*offsetX);
    const float v_offsetY   = (*offsetY);
    const uint  v_max_iters = (*max_iters);

    if( (position.x >= v_width) || (position.y >= v_height) )
    {
        return;
    }

    //
    // On calcule la position (x, y) de l'élement en cours de calcul
    //

    float zReal = v_offsetX + v_zoom * (float)(position.x);
    float zImag = v_offsetY + v_zoom * (float)(position.y);
    float startReal = zReal;
    float startImag = zImag;

    uint counter = 0;
#if 1
    for (; counter < v_max_iters; counter += 1)
    {
        const float r2 = zReal * zReal;
        const float i2 = zImag * zImag;

        zImag = 2.0f * zReal * zImag + startImag;
        zReal = r2 - i2 + startReal;

        if ( (r2 + i2) > 4.0f)
        {
            break;
        }
    }
#else
    float r2, i2;
    do {
        r2    = zReal * zReal;
        i2    = zImag * zImag;
        zImag = 2.0f * zReal * zImag + startImag;
        zReal = r2 - i2 + startReal;

    } while( (counter++ < v_max_iters) &&  ((r2 + i2) < 4.0) );
#endif

    threadgroup_barrier(mem_flags::mem_none);

    const uint v_pos = position.y * v_width + position.x;
    v_dat[v_pos]     = counter;
}
