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
#include "ColorSetCandy.hpp"

// https://github.com/elkayem/Mandelbrot-Explorer/blob/master/VHDL/colormap_candy.vhd

ColorSetCandy::ColorSetCandy(int max_iters) : ColorMap("ColorSetCandy")
{
    setIters(max_iters);
}


void ColorSetCandy::setIters(int max_iters)
{
    const uint32_t values[] = {
            0x0000, 0x0c82, 0x0c92, 0x0c92, 0x0c92,
            0x0d92, 0x0d92, 0x0d92, 0x0d92,
            0x0da2, 0x0ea2, 0x0ea1, 0x0ea1,
            0x0ea1, 0x0ea1, 0x0ea1, 0x0fa1,
            0x0fb1, 0x0fb1, 0x0fb1, 0x0fb1,
            0x0fb1, 0x0fa2, 0x0fa2, 0x0fa2,
            0x0f92, 0x0f92, 0x0f92, 0x0f82,
            0x0f83, 0x0f83, 0x0f73, 0x0f73,
            0x0f73, 0x0f73, 0x0f64, 0x0f64,
            0x0f64, 0x0f54, 0x0f54, 0x0f54,
            0x0f45, 0x0f45, 0x0f45, 0x0f35,
            0x0f35, 0x0f35, 0x0f25, 0x0f26,
            0x0f26, 0x0e36, 0x0d46, 0x0c56,
            0x0a66, 0x0987, 0x0897, 0x07a7,
            0x06b7, 0x05d7, 0x0f0f, 0x0e2d,
            0x0c5c, 0x0a8b, 0x09b9, 0x07e8,
            0x09b9, 0x09b9, 0x09b9, 0x0ab8,
            0x0ab8, 0x0ac8, 0x0bc7, 0x0bc7,
            0x0bc7, 0x0cc6, 0x0cc6, 0x0cc6,
            0x0dd6, 0x0dd5, 0x0dd5, 0x0ed5,
            0x0ed4, 0x0ed4, 0x0fd4, 0x0fd3,
            0x0fe3, 0x0fe3, 0x0fe3, 0x0fe2,
            0x0fe3, 0x0fe3,
            0x0fe3, 0x0fe3,
            0x0fd3, 0x0fd3,
            0x0fd3, 0x0fd3,
            0x0fd3, 0x0fd3,
            0x0fd4, 0x0fd4,
            0x0fc4, 0x0fc4,
            0x0fc4, 0x0fc4,
            0x0ec4, 0x0ec4,
            0x0ec4, 0x0ec4,
            0x0eb5, 0x0eb5,
            0x0eb5, 0x0eb5,
            0x0eb5, 0x0eb5,
            0x0eb5, 0x0db5,
            0x0db5, 0x0da5,
            0x0da5, 0x0da6,
            0x0da6, 0x0da6,
            0x0da6, 0x0da6,
            0x0da6, 0x0c96,
            0x0c96, 0x0c96,
            0x0c96, 0x0c97,
            0x0c97, 0x0c97,
            0x0c97, 0x0c97,
            0x0c87, 0x0c87,
            0x0b87, 0x0b87,
            0x0b87,
            0x0b87,
            0x0b88,
            0x0b88,
            0x0b78,
            0x0b78,
            0x0b78,
            0x0b78,
            0x0b78,
            0x0a78,
            0x0a78,
            0x0a78,
            0x0a69,
            0x0a69,
            0x0a69,
            0x0a69,
            0x0a69,
            0x0a69,
            0x0a69,
            0x0a69,
            0x0969,
            0x0959,
            0x0959,
            0x095a,
            0x095a,
            0x095a,
            0x095a,
            0x095a,
            0x095a,
            0x094a,
            0x094a,
            0x084a,
            0x084a,
            0x084b,
            0x084a,
            0x084a,
            0x083a,
            0x083a,
            0x082a,
            0x0829,
            0x0829,
            0x0819,
            0x0819,
            0x0819,
            0x0809,
            0x0808,
            0x0809,
            0x0819,
            0x0819,
            0x0819,
            0x0819,
            0x0818,
            0x0818,
            0x0818,
            0x0818,
            0x0828,
            0x0828,
            0x0828,
            0x0828,
            0x0828,
            0x0828,
            0x0828,
            0x0828,
            0x0838,
            0x0838,
            0x0838,
            0x0838,
            0x0838,
            0x0838,
            0x0838,
            0x0838,
            0x0848,
            0x0848,
            0x0547,
            0x0547,
            0x0547,
            0x0547,
            0x0547,
            0x0547,
            0x0557,
            0x0557,
            0x0557,
            0x0557,
            0x0557,
            0x0557,
            0x0557,
            0x0567,
            0x0567,
            0x0567,
            0x0567,
            0x0567,
            0x0567,
            0x0567,
            0x0567,
            0x0577,
            0x0577,
            0x0576,
            0x0576,
            0x0576,
            0x0576,
            0x0576,
            0x0576,
            0x0586,
            0x0586,
            0x0586,
            0x0586,
            0x0586,
            0x0586,
            0x0586,
            0x0586,
            0x0596,
            0x0596,
            0x0596,
            0x0596,
            0x0596,
            0x0596,
            0x0596
    };

    MAX = max_iters;

    for (int32_t i = 0; i < MAX; ++i)
    {
        const uint32_t v = values[sizeof(values) - i - 1];
        const uint32_t r = (v      & 0xF) << 4;
        const uint32_t g = (v >> 4 & 0xF) << 4;
        const uint32_t b = (v >> 8 & 0xF) << 4;
        colors[i]        = sf::Color(r, g, b);
    }
    colors[MAX-1] = sf::Color(0, 0, 0);
}


ColorSetCandy::~ColorSetCandy()
{

}
