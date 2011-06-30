#!/bin/sh

cmake -DCMAKE_BUILD_TYPE=Release -DCF_TEST=On -DCF_NO_IMAGE_MAGICK=On -G 'MSYS Makefiles' "`dirname $0`/../.."
