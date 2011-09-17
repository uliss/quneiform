#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import sys
import cf

INPUT_FILES = (
    'bmp16_1.bmp',
    'bmp16_2.bmp',
    'bmp24.bmp',
    'bmp32.bmp',
    'bmp_bw.bmp',
    'bmp_gray.bmp',
    'bmp_gray_rle.bmp',
    'bmp_indexed.bmp',
    'bmp_indexed_rle.bmp',
    'gif_gray.gif',
    'gif_indexed.gif',
    'jpeg_gray.jpg',
    'jpeg.jpg',
    'png32_alpha.png',
    'png32.png',
    'png_bw.png',
    'png_gray.png',
    'png_indexed.png',
    'pnm32.pnm',
    'pnm_bw.pbm',
    'pnm_gray.pgm',
    'tiff32_dim.tiff',
    'tiff32_jpeg.tiff',
    'tiff32_lzw.tiff',
    'tiff32_packed_bits.tiff',
    'tiff32.tiff',
    'tiff_bw_dim.tiff',
    'tiff_bw_fax3.tiff',
    'tiff_bw_fax4.tiff',
    'tiff_bw_lzw.tiff',
    'tiff_bw_packed_bits.tiff',
    'tiff_bw.tiff',
    'tiff_gray_dim.tiff',
    'tiff_gray_jpeg.tiff',
    'tiff_gray_lzw.tiff',
    'tiff_gray_packed_bits.tiff',
    'tiff_gray.tiff',
    'tiff_indexed_dim.tiff',
    'tiff_indexed_lzw.tiff',
    'tiff_indexed_packed_bits.tiff',
    'tiff_indexed.tiff'
)

def test():
    fmtTest = cf.Tester('image_formats')
    fmtTest.setFormat('textdebug')
    
    for img in INPUT_FILES:
        fmtTest.cuneiformTest(fmtTest.makeFullImageName(img))
        
    fmtTest.removeOutput(INPUT_FILES, ['textdebug'])

    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

if __name__ == '__main__':
    test()

