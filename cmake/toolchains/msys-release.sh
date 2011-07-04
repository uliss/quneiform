#!/bin/sh

cmake -DCMAKE_BUILD_TYPE=Release -DCF_TEST=OFF -DCF_NO_IMAGE_MAGICK=On -G 'MSYS Makefiles' "`dirname $0`/../.."
