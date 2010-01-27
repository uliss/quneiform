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
#include "image.h"
#include "rdib/bmp.h"

namespace CIF
{

class BmpImageLoader: public ImageLoader
{
    public:
        BmpImageLoader();
        ~BmpImageLoader();
        ImagePtr load(const std::string& fname);
        ImagePtr load(std::istream& stream);
    private:
        void allocateColorTable();
        void convertColorSpace();
        bool isValidBmpBitCount();
        bool read(std::istream& os);
        void readBmpMagick(std::istream& stream);
        void readBmpFileHeader(std::istream& stream);
        void readBmpInfoHeader(std::istream& stream);
        void readColorTable(std::istream& stream);
        void readCompressedData(std::istream& stream);
        void readData(std::istream& stream);
        void readUncompressedData(std::istream& stream);
        int stride() const
        {
            return (w * spp * bps + 7) / 8;
        }
    private:
        BMPFileHeader file_header_;
        BMPInfoHeader info_header_;
        BMPType bmp_type;
        uint32_t n_clr_elems;
        uint bps;
        uint spp;
        uint32_t clr_tbl_size;
        uint8_t * clr_tbl;
        uint w;
        uint h;
        char * data_;
        int data_size_;
};

}

#endif /* BMPIMAGELOADER_H_ */
