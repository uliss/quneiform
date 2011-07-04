#!/bin/sh

cmake -DCMAKE_BUILD_TYPE=Debug -DCF_OCRTK=On -DTESTS=On -DCF_NO_IMAGE_MAGICK=On "`dirname $0`/../../" -G 'MSYS Makefiles'
