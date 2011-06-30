#!/bin/sh

cmake -DCMAKE_BUILD_TYPE=Debug -DCF_OCRTK=On -DCF_TEST=On -DCF_NO_IMAGE_MAGICK=On "`dirname $0`/../../" -G 'MSYS Makefiles'
