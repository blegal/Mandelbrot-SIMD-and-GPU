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
//
// Ce fichier est embarque dans l'executable au moment de la compilation (cf.
// CMakeLists.txt) puis compile a la volee par le driver Metal : il n'est donc
// pas necessaire d'installer la "Metal Toolchain" d'Xcode.
//
#include <metal_stdlib>
using namespace metal;
/*
 *
 *
 *##############################################################################
 *
 *
 */
struct sp_params {
    float zoom;      // Le facteur de zoom (taille d'un pixel)
    float cornerX;   // la coordonnée X du coin superieur gauche
    float cornerY;   // la coordonnée Y du coin superieur gauche
    uint  width;     // la largeur de l'image
    uint  height;    // la hauteur de l'image
    uint  max_iters; // le nombre maximum d'itérations
};
/*
 *
 *
 *##############################################################################
 *
 *
 */
kernel void mandelbrot_sp(
          device float*     v_dat  [[buffer(0)]], // le pointeur sur le tableau de sortie
    constant sp_params&     p      [[buffer(1)]], // les parametres de la vue
    uint2 position [[thread_position_in_grid]]    // La position du calcul à réaliser
)
{
    //
    // Si jamais notre position dans la grille déborde
    //
    if( (position.x >= p.width) || (position.y >= p.height) )
    {
        return;
    }

    //
    // On calcule la position (x, y) de l'élement en cours de calcul
    //

    const float startReal = p.cornerX + p.zoom * (float)(position.x);
    const float startImag = p.cornerY + p.zoom * (float)(position.y);

    float zReal = startReal;
    float zImag = startImag;

    uint counter = 0;
    for (; counter < p.max_iters; counter += 1)
    {
        const float r2 = zReal * zReal;
        const float i2 = zImag * zImag;

        if ( (r2 + i2) > 4.0f)
        {
            break;
        }

        zImag = 2.0f * zReal * zImag + startImag;
        zReal = r2 - i2 + startReal;
    }

    v_dat[position.y * p.width + position.x] = (float)counter;
}
/*
 *
 *
 *##############################################################################
 *
 *
 */
//
// Arithmetique "float-float" (ou df64) : un nombre est represente par la somme
// non evaluee de deux floats (hi + lo), soit ~48 bits de mantisse. Les GPU Apple
// ne disposant pas de double precision materielle, cette emulation permet de
// zoomer environ 10^6 fois plus profondement qu'en simple precision.
//
// ATTENTION : ce code doit etre compile SANS fast-math (cf. MetalEngine.cpp).
//
struct df_params {
    float2 zoom;     // (hi, lo)
    float2 cornerX;  // (hi, lo)
    float2 cornerY;  // (hi, lo)
    uint   width;
    uint   height;
    uint   max_iters;
};

static inline float2 quick_two_sum(const float a, const float b)
{
    const float s = a + b;
    const float e = b - (s - a);
    return float2(s, e);
}

static inline float2 two_sum(const float a, const float b)
{
    const float s = a + b;
    const float v = s - a;
    const float e = (a - (s - v)) + (b - v);
    return float2(s, e);
}

static inline float2 two_prod(const float a, const float b)
{
    const float p = a * b;
    const float e = fma(a, b, -p);
    return float2(p, e);
}

static inline float2 df_add(const float2 a, const float2 b)
{
    float2 s = two_sum(a.x, b.x);
    s.y += a.y + b.y;
    return quick_two_sum(s.x, s.y);
}

static inline float2 df_sub(const float2 a, const float2 b)
{
    return df_add(a, float2(-b.x, -b.y));
}

static inline float2 df_mul(const float2 a, const float2 b)
{
    float2 p = two_prod(a.x, b.x);
    p.y += a.x * b.y + a.y * b.x;
    return quick_two_sum(p.x, p.y);
}

static inline float2 df_mul_f(const float2 a, const float b)
{
    float2 p = two_prod(a.x, b);
    p.y += a.y * b;
    return quick_two_sum(p.x, p.y);
}

kernel void mandelbrot_df(
          device float*     v_dat  [[buffer(0)]], // le pointeur sur le tableau de sortie
    constant df_params&     p      [[buffer(1)]], // les parametres de la vue
    uint2 position [[thread_position_in_grid]]    // La position du calcul à réaliser
)
{
    if( (position.x >= p.width) || (position.y >= p.height) )
    {
        return;
    }

    const float2 startReal = df_add(p.cornerX, df_mul_f(p.zoom, (float)position.x));
    const float2 startImag = df_add(p.cornerY, df_mul_f(p.zoom, (float)position.y));

    float2 zReal = startReal;
    float2 zImag = startImag;

    uint counter = 0;
    for (; counter < p.max_iters; counter += 1)
    {
        const float2 r2 = df_mul(zReal, zReal);
        const float2 i2 = df_mul(zImag, zImag);

        if ( (r2.x + i2.x) > 4.0f)
        {
            break;
        }

        const float2 ri = df_mul(zReal, zImag);
        zImag = df_add(df_add(ri, ri), startImag);
        zReal = df_add(df_sub(r2, i2), startReal);
    }

    v_dat[position.y * p.width + position.x] = (float)counter;
}
