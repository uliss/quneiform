/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky, serge.poltavski@gmail.com      *
 *   based on  exactImage library                                          *
 *   Copyright (C) 2006 - 2009 René Rebe, ExactCODE GmbH Germany           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <boost/scoped_array.hpp>
#include <cstring>
#include <fstream>

#include "bmpimageloader.h"
#include "imageloaderfactory.h"
#include "common/debug.h"

namespace
{
CIF::ImageLoader * create()
{
    return new CIF::BmpImageLoader;
}

bool registered = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_BMP, 0, create);

}

namespace CIF
{

int last_bit_set(int v)
{
    for (uint i = sizeof(int) * 8 - 1; i > 0; --i) {
        if (v & (1L << i))
            return i;
    }
    return 0;
}

BmpImageLoader::BmpImageLoader() :
    clr_tbl_size(0), clr_tbl(NULL)
{
}

BmpImageLoader::~BmpImageLoader()
{
}

void BmpImageLoader::allocateColorTable()
{
    /* Allocate memory for colour table and read it. */
    if (info_header_.iClrUsed)
        clr_tbl_size = ((uint32_t) (1 << bps) < info_header_.iClrUsed)
                ? 1 << bps
                : info_header_.iClrUsed;
    else
        clr_tbl_size = 1 << bps;
    clr_tbl = (uint8_t *) new char[n_clr_elems * clr_tbl_size];

    /*printf ("n_clr_elems: %d, clr_tbl_size: %d\n",
     n_clr_elems, clr_tbl_size); */
}

Image* BmpImageLoader::load(const std::string& fname)
{
    std::ifstream stream(fname.c_str(), std::ios::binary | std::ios::in);
    if (!stream.good())
        throw ImageLoader::Exception("Invalid input stream given");

    readBmpMagick(stream);
    readBmpFileHeader(stream);
    readBmpInfoHeader(stream);
    if (!isValidBmpBitCount())
        throw ImageLoader::Exception("Unsupported image bit-depth.");

    readData(stream);

    return new Image(data_, data_size_, Image::AllocatorMalloc);
}

void BmpImageLoader::convertColorSpace()
{
    uint16_t* rmap = new uint16_t[1 << bps];
    uint16_t* gmap = new uint16_t[1 << bps];
    uint16_t* bmap = new uint16_t[1 << bps];

    for (int i = 0; i < (1 << bps); ++i) {
        // BMP maps have BGR order ...
        rmap[i] = 0x101 * clr_tbl[i * n_clr_elems + 2];
        gmap[i] = 0x101 * clr_tbl[i * n_clr_elems + 1];
        bmap[i] = 0x101 * clr_tbl[i * n_clr_elems + 0];
    }

    //    colorspace_de_palette(image, clr_tbl_size, rmap, gmap, bmap);

    delete[] rmap;
    delete[] gmap;
    delete[] bmap;
    delete[] clr_tbl;
    clr_tbl = NULL;
}

bool BmpImageLoader::isValidBmpBitCount()
{
    switch (info_header_.iBitCount) {
    case 1:
    case 4:
    case 8:
    case 16:
    case 24:
    case 32:
        return true;
    default:
        Debug() << "BMPCodec:: Cannot read " << info_header_.iBitCount << " bit files.\n";
        return false;
    }
}

void BmpImageLoader::readBmpMagick(std::istream& stream)
{
    stream.read((char*) &file_header_.bType, 2);
    if (file_header_.bType[0] != 'B' || file_header_.bType[1] != 'M') {
        stream.seekg(0);
        throw ImageLoader::Exception("Not a BMP file");
    }
}

void BmpImageLoader::readBmpFileHeader(std::istream& stream)
{
    stream.seekg(10);
    stream.read((char*) &file_header_.iOffBits, 4);
    // fix the iSize, in early BMP file this is pure garbage
    file_header_.iSize = (uint32_t) streamSize(stream);
}

void BmpImageLoader::readBmpInfoHeader(std::istream& stream)
{
    stream.seekg(BFH_SIZE);
    stream.read((char*) &info_header_.iSize, 4);

    n_clr_elems = 3;

    switch (info_header_.iSize) {
    case BIH_WIN4SIZE:
        bmp_type = BMPT_WIN4;
        n_clr_elems = 4;
        Debug() << "Windows V3\n";
        break;
    case BIH_OS21SIZE:
        bmp_type = BMPT_OS21;
        Debug() << "OS/2 V1\n";
        break;
    case BIH_OS22SIZE:
    case 16:
        bmp_type = BMPT_OS22;
        Debug() << "OS/2 V2\n";
        break;
    case 108:
        bmp_type = BMPT_WIN5;
        n_clr_elems = 4;
        Debug() << "Windows V4\n";
        break;
    case 124:
        bmp_type = BMPT_WIN5;
        n_clr_elems = 4;
        Debug() << "Windows V5\n";
        break;
    default:
        throw Exception("Unknown BMP version");
    }

    if (bmp_type == BMPT_WIN4 || bmp_type == BMPT_WIN5 || bmp_type == BMPT_OS22) {
        stream.read((char*) &info_header_.iWidth, 4);
        stream.read((char*) &info_header_.iHeight, 4);
        stream.read((char*) &info_header_.iPlanes, 2);
        stream.read((char*) &info_header_.iBitCount, 2);
        stream.read((char*) &info_header_.iCompression, 4);
        stream.read((char*) &info_header_.iSizeImage, 4);
        stream.read((char*) &info_header_.iXPelsPerMeter, 4);
        stream.read((char*) &info_header_.iYPelsPerMeter, 4);
        stream.read((char*) &info_header_.iClrUsed, 4);
        stream.read((char*) &info_header_.iClrImportant, 4);
        stream.read((char*) &info_header_.iRedMask, 4);
        stream.read((char*) &info_header_.iGreenMask, 4);
        stream.read((char*) &info_header_.iBlueMask, 4);
        stream.read((char*) &info_header_.iAlphaMask, 4);
    }

    if (bmp_type == BMPT_OS21) {
        int16_t iShort;

        stream.read((char*) &iShort, 2);
        info_header_.iWidth = iShort;
        stream.read((char*) &iShort, 2);
        info_header_.iHeight = iShort;
        stream.read((char*) &iShort, 2);
        info_header_.iPlanes = iShort;
        stream.read((char*) &iShort, 2);
        info_header_.iBitCount = iShort;
        info_header_.iCompression = BMPC_RGB;
    }
}

void BmpImageLoader::readColorTable(std::istream& stream)
{
    allocateColorTable();

    stream.seekg(BFH_SIZE + info_header_.iSize);
    stream.read((char*) clr_tbl, n_clr_elems * clr_tbl_size);
}

void BmpImageLoader::readCompressedData(std::istream& stream)
{
    uint32_t i, j, k, runlength, x;
    uint32_t compr_size, uncompr_size;
    uint8_t *comprbuf;
    uint8_t *uncomprbuf;

    //std::cerr << "RLE" << (info_header_.iCompression == BMPC_RLE4 ? "4" : "8")
    //    << " compressed\n";

    compr_size = file_header_.iSize - file_header_.iOffBits;
    uncompr_size = w * h;

    comprbuf = (uint8_t *) malloc(compr_size);
    if (!comprbuf) {
        std::cerr << "Can't allocate space for compressed scanline buffer\n";
        if (clr_tbl)
            delete[] clr_tbl;
        clr_tbl = NULL;
    }
    uncomprbuf = (uint8_t *) malloc(uncompr_size);
    if (!uncomprbuf) {
        std::cerr << "Can't allocate space for uncompressed scanline buffer\n";
        if (clr_tbl)
            delete[] clr_tbl;
        clr_tbl = NULL;
    }

    stream.seekg(*file_header_.iOffBits);
    stream.read((char*) comprbuf, compr_size);
    i = j = x = 0;

    while (j < uncompr_size && i < compr_size) {
        if (comprbuf[i]) {
            runlength = comprbuf[i++];
            for (k = 0; runlength > 0 && j < uncompr_size && i < compr_size && x < (uint32_t) w; ++k, ++x) {
                if (info_header_.iBitCount == 8)
                    uncomprbuf[j++] = comprbuf[i];
                else {
                    if (k & 0x01)
                        uncomprbuf[j++] = comprbuf[i] & 0x0F;
                    else
                        uncomprbuf[j++] = (comprbuf[i] & 0xF0) >> 4;
                }
                runlength--;
            }
            i++;
        }
        else {
            i++;
            if (comprbuf[i] == 0) { /* Next scanline */
                i++;
                x = 0;
                ;
            }
            else if (comprbuf[i] == 1) /* End of image */
                break;
            else if (comprbuf[i] == 2) { /* Move to... */
                i++;
                if (i < compr_size - 1) {
                    j += comprbuf[i] + comprbuf[i + 1] * w;
                    i += 2;
                }
                else
                    break;
            }
            else { /* Absolute mode */
                runlength = comprbuf[i++];
                for (k = 0; k < runlength && j < uncompr_size && i < compr_size; k++, x++) {
                    if (info_header_.iBitCount == 8)
                        uncomprbuf[j++] = comprbuf[i++];
                    else {
                        if (k & 0x01)
                            uncomprbuf[j++] = comprbuf[i++] & 0x0F;
                        else
                            uncomprbuf[j++] = (comprbuf[i] & 0xF0) >> 4;
                    }
                }
                /* word boundary alignment */
                if (info_header_.iBitCount == 4)
                    k /= 2;
                if (k & 0x01)
                    i++;
            }
        }
    }

    free(comprbuf);
    data_size_ = info_header_.iSize + uncompr_size;
    data_ = (char *) malloc(data_size_);
    uint8_t * data = data + info_header_.iSize;
    if (!data_) {
        std::cerr << "Can't allocate space for final uncompressed scanline buffer\n";
        if (clr_tbl)
            delete[] clr_tbl;
        clr_tbl = NULL;
    }

    // TODO: suboptimal, later improve the above to yield the corrent orientation natively
    for (size_t row = 0; row < (uint32_t) h; ++row) {
        memcpy(data + row * w, uncomprbuf + (h - 1 - row) * w, w);
        //        rearrangePixels(data + row * w, w, info_header_.iBitCount);
    }

    free(uncomprbuf);
    bps = 8;
}

void BmpImageLoader::readData(std::istream& stream)
{
    switch (info_header_.iCompression) {
    //    case BMPC_RLE4:
    //    case BMPC_RLE8:
    //        readCompressedData(stream);
    //        break;
    case BMPC_RGB:
        readUncompressedData(stream);
        break;
//    case BMPC_BITFIELDS:
//        // we unpack bitfields to plain RGB
//        bps = 8;
    default:
        throw Exception("Unknown compression method");
    }
}

void BmpImageLoader::readUncompressedData(std::istream& stream)
{
    data_size_ = streamSize(stream) - sizeof(BMPFileHeader);
    assert(data_size_ > 0);
    data_ = new char[data_size_];
    stream.seekg(sizeof(BMPFileHeader));
    stream.read(data_, data_size_);
}

bool BmpImageLoader::read(std::istream& stream)
{
    if (!stream.good())
        throw ImageLoader::Exception("Invalid input stream given");

    readBmpMagick(stream);
    readBmpFileHeader(stream);
    readBmpInfoHeader(stream);
    if (!isValidBmpBitCount())
        throw ImageLoader::Exception("Unsupported image bit-depth.");

    switch (info_header_.iBitCount) {
    case 1:
    case 2:
    case 4:
    case 8:
        spp = 1;
        bps = info_header_.iBitCount;

        readColorTable(stream);
        break;
    case 16:
    case 24:
        spp = 3;
        bps = info_header_.iBitCount / spp;
        break;
    case 32:
        spp = 3;
        bps = 8;
        break;
    default:
        break;
    }

    // detect old style bitmask images
    if (info_header_.iCompression == BMPC_RGB && info_header_.iBitCount == 16) {
        //std::cerr << "implicit non-RGB image\n";
        info_header_.iCompression = BMPC_BITFIELDS;
        info_header_.iBlueMask = 0x1f;
        info_header_.iGreenMask = 0x1f << 5;
        info_header_.iRedMask = 0x1f << 10;
    }

    readData(stream);

    // no color table anyway or RGB* ?
    if (clr_tbl && spp < 3)
        convertColorSpace();

    return true;
}

}
