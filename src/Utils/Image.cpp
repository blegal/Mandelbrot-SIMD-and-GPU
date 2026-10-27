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
#include "Image.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>

namespace gfx {

static_assert(sizeof(Color) == 4, "gfx::Color must be tightly packed (RGBA8)");


Image::Image() : _width(0), _height(0)
{

}


void Image::create(const uint32_t width, const uint32_t height, const Color& color)
{
    _width  = width;
    _height = height;
    pixels.assign((size_t)width * height, color);
}


const uint8_t* Image::getPixelsPtr() const
{
    return reinterpret_cast<const uint8_t*>( pixels.data() );
}


bool Image::saveToFile(const std::string& filename) const
{
    std::string ext = filename.substr(filename.find_last_of('.') + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c){ return std::tolower(c); });

    if( ext == "ppm" ) return savePPM( filename );
    if( ext == "bmp" ) return saveBMP( filename );
    return savePNG( filename );
}


bool Image::savePPM(const std::string& filename) const
{
    std::ofstream f(filename, std::ios::binary);
    if( !f ) return false;
    f << "P6\n" << _width << " " << _height << "\n255\n";
    for(const Color& c : pixels)
    {
        f.put( c.r );
        f.put( c.g );
        f.put( c.b );
    }
    return f.good();
}


static void put_le16(std::ofstream& f, const uint16_t v) { f.put(v & 0xFF); f.put(v >> 8); }
static void put_le32(std::ofstream& f, const uint32_t v) { put_le16(f, v & 0xFFFF); put_le16(f, v >> 16); }


bool Image::saveBMP(const std::string& filename) const
{
    std::ofstream f(filename, std::ios::binary);
    if( !f ) return false;

    const uint32_t row  = (3 * _width + 3) & ~3u; // les lignes BMP sont alignees sur 4 octets
    const uint32_t size = row * _height;

    f.put('B'); f.put('M');
    put_le32(f, 54 + size); put_le32(f, 0); put_le32(f, 54);
    put_le32(f, 40); put_le32(f, _width); put_le32(f, _height);
    put_le16(f, 1);  put_le16(f, 24);     put_le32(f, 0);
    put_le32(f, size); put_le32(f, 2835); put_le32(f, 2835);
    put_le32(f, 0);  put_le32(f, 0);

    for(int32_t y = _height - 1; y >= 0; y -= 1) // BMP : de bas en haut
    {
        for(uint32_t x = 0; x < _width; x += 1)
        {
            const Color& c = pixels[y * _width + x];
            f.put( c.b );
            f.put( c.g );
            f.put( c.r );
        }
        for(uint32_t p = 3 * _width; p < row; p += 1)
            f.put( 0 );
    }
    return f.good();
}


//
// Encodeur PNG minimaliste : le flux zlib utilise des blocs "stored" (non
// compresses), ce qui evite toute dependance externe (libpng, zlib, ...).
//

static uint32_t crc32(const uint8_t* data, const size_t length, uint32_t crc = 0)
{
    static uint32_t table[256] = { 0 };
    if( table[1] == 0 )
    {
        for(uint32_t n = 0; n < 256; n += 1)
        {
            uint32_t c = n;
            for(int k = 0; k < 8; k += 1)
                c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            table[n] = c;
        }
    }
    crc = ~crc;
    for(size_t i = 0; i < length; i += 1)
        crc = table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    return ~crc;
}


static void put_be32(std::vector<uint8_t>& v, const uint32_t x)
{
    v.push_back( x >> 24 ); v.push_back( x >> 16 ); v.push_back( x >> 8 ); v.push_back( x );
}


static void write_chunk(std::ofstream& f, const char* type, const std::vector<uint8_t>& data)
{
    std::vector<uint8_t> chunk;
    put_be32(chunk, (uint32_t)data.size());
    chunk.insert(chunk.end(), type, type + 4);
    chunk.insert(chunk.end(), data.begin(), data.end());
    put_be32(chunk, crc32(chunk.data() + 4, chunk.size() - 4));
    f.write((const char*)chunk.data(), chunk.size());
}


bool Image::savePNG(const std::string& filename) const
{
    std::ofstream f(filename, std::ios::binary);
    if( !f ) return false;

    const uint8_t signature[8] = { 0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n' };
    f.write((const char*)signature, 8);

    std::vector<uint8_t> ihdr;
    put_be32(ihdr, _width);
    put_be32(ihdr, _height);
    ihdr.push_back( 8 ); // 8 bits par composante
    ihdr.push_back( 2 ); // RGB
    ihdr.push_back( 0 ); ihdr.push_back( 0 ); ihdr.push_back( 0 );
    write_chunk(f, "IHDR", ihdr);

    //
    // Donnees brutes : un octet de filtre (0) en debut de chaque ligne
    //
    std::vector<uint8_t> raw;
    raw.reserve((size_t)_height * (3 * _width + 1));
    for(uint32_t y = 0; y < _height; y += 1)
    {
        raw.push_back( 0 );
        for(uint32_t x = 0; x < _width; x += 1)
        {
            const Color& c = pixels[y * _width + x];
            raw.push_back( c.r ); raw.push_back( c.g ); raw.push_back( c.b );
        }
    }

    std::vector<uint8_t> idat;
    idat.push_back( 0x78 ); idat.push_back( 0x01 ); // en-tete zlib
    uint32_t a = 1, b = 0;                         // adler32
    for(size_t pos = 0; pos < raw.size() || pos == 0; )
    {
        const size_t   len  = std::min<size_t>(65535, raw.size() - pos);
        const bool     last = (pos + len) == raw.size();
        idat.push_back( last ? 1 : 0 );
        idat.push_back( len & 0xFF );  idat.push_back( len >> 8 );
        idat.push_back( ~len & 0xFF ); idat.push_back( (~len >> 8) & 0xFF );
        for(size_t i = pos; i < pos + len; i += 1)
        {
            a = (a + raw[i]) % 65521;
            b = (b + a     ) % 65521;
        }
        idat.insert(idat.end(), raw.begin() + pos, raw.begin() + pos + len);
        pos += len;
        if( last ) break;
    }
    put_be32(idat, (b << 16) | a);
    write_chunk(f, "IDAT", idat);
    write_chunk(f, "IEND", std::vector<uint8_t>());

    return f.good();
}

}
