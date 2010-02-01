/*
 * C++ BMP library.
 * Copyright (C) 2006 - 2009 René Rebe, ExactCODE GmbH Germany
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2. A copy of the GNU General
 * Public License can be found in the file LICENSE.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANT-
 * ABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * Alternatively, commercial licensing options are available from the
 * copyright holder ExactCODE GmbH Germany.
 *
 * Lossy based on (in the past more so, but more and more parts got
 * rewritten):
 *
 * Project:  libtiff tools
 * Purpose:  Convert Windows BMP files in TIFF.
 * Author:   Andrey Kiselev, dron@remotesensing.org
 */

#ifndef BMP_H_
#define BMP_H_

#include "common/endianess.h"

using Exact::EndianessConverter;
using Exact::LittleEndianTraits;

namespace CIF
{

enum BMPType
{
    BMPT_WIN4, /* BMP used in Windows 3.0/NT 3.51/95 */
    BMPT_WIN5, /* BMP used in Windows NT 4.0/98/Me/2000/XP */
    BMPT_OS21, /* BMP used in OS/2 PM 1.x */
    BMPT_OS22
/* BMP used in OS/2 PM 2.x */
};

/*
 * Bitmap file consists of a BMPFileHeader structure followed by a
 * BMPInfoHeader structure. An array of BMPColorEntry structures (also called
 * a color table) follows the bitmap information header structure. The color
 * table is followed by a second array of indexes into the color table (the
 * actual bitmap data). Data may be compressed, for 4-bpp and 8-bpp used RLE
 * compression.
 *
 * +---------------------+
 * | BMPFileHeader       |
 * +---------------------+
 * | BMPInfoHeader       |
 * +---------------------+
 * | BMPColorEntry array |
 * +---------------------+
 * | Colour-index array  |
 * +---------------------+
 *
 * All numbers stored in Intel order with least significant byte first.
 */

enum BMPComprMethod
{
    BMPC_RGB = 0L, /* Uncompressed */
    BMPC_RLE8 = 1L, /* RLE for 8 bpp images */
    BMPC_RLE4 = 2L, /* RLE for 4 bpp images */
    BMPC_BITFIELDS = 3L, /* Bitmap is not compressed and the color table
     * consists of three DWORD color masks that specify
     * the red, green, and blue components of each
     * pixel. This is valid when used with
     * 16- and 32-bpp bitmaps. */
    BMPC_JPEG = 4L, /* Indicates that the image is a JPEG image. */
    BMPC_PNG = 5L
/* Indicates that the image is a PNG image. */
};

enum BMPLCSType /* Type of logical color space. */
{
    BMPLT_CALIBRATED_RGB = 0, /* This value indicates that endpoints and
     * gamma values are given in the appropriate
     * fields. */
    BMPLT_DEVICE_RGB = 1,
    BMPLT_DEVICE_CMYK = 2
};

struct BMPCIEXYZ
{
        int32_t iCIEX;
        int32_t iCIEY;
        int32_t iCIEZ;
};

struct BMPCIEXYZTriple /* This structure contains the x, y, and z */
{ /* coordinates of the three colors that */
        /* correspond */
        BMPCIEXYZ iCIERed; /* to the red, green, and blue endpoints for */
        BMPCIEXYZ iCIEGreen; /* a specified logical color space. */
        BMPCIEXYZ iCIEBlue;
};

PACKED_STRUCT_PROLOGUE
struct BMPFileHeader
{
        char bType[2]; /* Signature "BM" */
        EndianessConverter<uint32_t, LittleEndianTraits> iSize; /* Size in bytes of the bitmap file. Should
         * always be ignored while reading because
         * of error in Windows 3.0 SDK's description
         * of this field */
        uint16_t iReserved1; /* Reserved, set as 0 */
        uint16_t iReserved2; /* Reserved, set as 0 */
        EndianessConverter<uint32_t, LittleEndianTraits> iOffBits; /* Offset of the image from file start in bytes */
} PACKED_STRUCT_EPILOGUE;

/* File header size in bytes: */
const int BFH_SIZE = 14;

PACKED_STRUCT_PROLOGUE
struct BMPInfoHeader
{
        EndianessConverter<uint32_t, LittleEndianTraits> iSize;
        /* Size of BMPInfoHeader structure in bytes.
         * Should be used to determine start of the
         * color table */
        EndianessConverter<int32_t, LittleEndianTraits> iWidth; /* Image width */
        EndianessConverter<int32_t, LittleEndianTraits> iHeight;
        /* Image height. If positive, image has bottom
         * left origin, if negative --- top left. */
        EndianessConverter<int16_t, LittleEndianTraits> iPlanes; /* Number of image planes (must be set to 1) */
        EndianessConverter<int16_t, LittleEndianTraits> iBitCount; /* Number of bits per pixel (1, 4, 8, 16, 24
         * or 32). If 0 then the number of bits per
         * pixel is specified or is implied by the
         * JPEG or PNG format. */
        EndianessConverter<uint32_t, LittleEndianTraits> iCompression; /* Compression method */
        EndianessConverter<uint32_t, LittleEndianTraits> iSizeImage;
        /* Size of uncompressed image in bytes. May
         * be 0 for BMPC_RGB bitmaps. If iCompression
         * is BI_JPEG or BI_PNG, iSizeImage indicates
         * the size of the JPEG or PNG image buffer. */
        EndianessConverter<int32_t, LittleEndianTraits> iXPelsPerMeter; /* X resolution, pixels per meter (0 if not used) */
        EndianessConverter<int32_t, LittleEndianTraits> iYPelsPerMeter; /* Y resolution, pixels per meter (0 if not used) */
        EndianessConverter<uint32_t, LittleEndianTraits> iClrUsed;
        /* Size of color table. If 0, iBitCount should
         * be used to calculate this value
         * (1<<iBitCount). This value should be
         * unsigned for proper shifting. */
        EndianessConverter<int32_t, LittleEndianTraits> iClrImportant;
        /* Number of important colors. If 0, all
         * colors are required */

        /*
         * Fields above should be used for bitmaps, compatible with Windows NT 3.51
         * and earlier. Windows 98/Me, Windows 2000/XP introduces additional fields:
         */

        EndianessConverter<int32_t, LittleEndianTraits> iRedMask;
        /* Color mask that specifies the red component
         * of each pixel, valid only if iCompression
         * is set to BI_BITFIELDS. */
        EndianessConverter<int32_t, LittleEndianTraits> iGreenMask; /* The same for green component */
        EndianessConverter<int32_t, LittleEndianTraits> iBlueMask; /* The same for blue component */
        EndianessConverter<int32_t, LittleEndianTraits> iAlphaMask;
        /* Color mask that specifies the alpha
         * component of each pixel. */
        EndianessConverter<uint32_t, LittleEndianTraits> iCSType; /* Color space of the DIB. */
        BMPCIEXYZTriple sEndpoints;
        /* This member is ignored unless the iCSType
         * member specifies BMPLT_CALIBRATED_RGB. */
        EndianessConverter<int32_t, LittleEndianTraits> iGammaRed;
        /* Toned response curve for red. This member
         * is ignored unless color values are
         * calibrated RGB values and iCSType is set to
         * BMPLT_CALIBRATED_RGB. Specified
         * in 16^16 format. */
        EndianessConverter<int32_t, LittleEndianTraits> iGammaGreen; /* Toned response curve for green. */
        EndianessConverter<int32_t, LittleEndianTraits> iGammaBlue; /* Toned response curve for blue. */
} PACKED_STRUCT_EPILOGUE;

/*
 * Info header size in bytes:
 */
static const unsigned int BIH_WIN4SIZE = 40; /* for BMPT_WIN4 */
static const unsigned int BIH_WIN5SIZE = 56; /* for BMPT_WIN5 */
static const unsigned int BIH_OS21SIZE = 12; /* for BMPT_OS21 */
static const unsigned int BIH_OS22SIZE = 64; /* for BMPT_OS22 */
static const unsigned int BIH_VER3SIZE = 40;
static const unsigned int BIH_VER4SIZE = 108;
static const unsigned int BIH_VER5SIZE = 124;

/*
 * We will use plain byte array instead of this structure, but declaration
 * provided for reference
 */
typedef struct
{
        char bBlue;
        char bGreen;
        char bRed;
        char bReserved; /* Must be 0 */
} BMPColorEntry;

}

#endif /* BMP_H_ */
