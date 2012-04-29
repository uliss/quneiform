/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
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

#ifndef BMPIMAGELOADER_H_
#define BMPIMAGELOADER_H_

#include "cttypes.h"
#include "imageloader.h"
#include "common/image.h"
#include "rdib/bmp.h"

namespace cf
{

class CLA_EXPO BmpImageLoader: public ImageLoader
{
    public:
        BmpImageLoader();
        ~BmpImageLoader();
        ImagePtr load(const std::string& fname);
        ImagePtr load(std::istream& stream);
        Handle loadHandle(const std::string& fname);
        Handle loadHandle(std::istream& stream);
    private:
        ImagePtr load_(std::istream& stream, ImageRawData::allocator_t allocator);
        void allocateColorTable();
        void convertColorSpace();
        uint imageBitCount() const;
        uint imageBitsPerStride() const;
        uint imageHeight() const;
        uint imageRowStride() const;
        uint imageStridePerPixel() const;
        uint imageWidth() const;
        bool isValidBmpBitCount();
        bool read(std::istream& stream);
        void readBitFieldData(std::istream& is);
        void readBmpMagick(std::istream& stream);
        void readBmpFileHeader(std::istream& stream);
        void readBmpInfoHeader(std::istream& stream);
        void readBmpInfoHeaderVersion(std::istream& stream);
        void readColorTable(std::istream& stream);
        void readCompressedData(std::istream& stream);
        void readData(std::istream& stream);
        void readInfoHeaderModern(std::istream& stream);
        void readInfoHeaderOs2v1(std::istream& stream);
        void readUncompressedData(std::istream& stream);
        uint stride() const;
    private:
        BMPFileHeader file_header_;
        BMPInfoHeader info_header_;
        BMPType bmp_type;
        uint32_t n_clr_elems;
        uint32_t clr_tbl_size;
        uint8_t * clr_tbl;
        unsigned char * data_;
        int data_size_;
};

inline uint BmpImageLoader::imageBitCount() const {
    return info_header_.iBitCount;
}

inline uint BmpImageLoader::imageHeight() const {
    return abs(info_header_.iHeight);
}

inline uint BmpImageLoader::imageWidth() const {
    return info_header_.iWidth;
}

inline uint BmpImageLoader::stride() const {
    return (imageWidth() * imageStridePerPixel() * imageBitsPerStride() + 7) / 8;
}

}

#endif /* BMPIMAGELOADER_H_ */
