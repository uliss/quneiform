#!/bin/sh
# Author: Serj Poltavski 

QF_BUNDLE="`echo "$0" | sed -e 's/\/Contents\/MacOS\/Quneiform//'`"
QF_RESOURCES="$QF_BUNDLE/Contents/Resources"

echo "running $0"
echo "QF_BUNDLE: $QF_BUNDLE"

#export "DYLD_LIBRARY_PATH=$QF_RESOURCES/lib"
export "PATH=$QF_RESOURCES/bin:$PATH"
export "CF_DATADIR=$QF_RESOURCES/share/cuneiform"

exec "$QF_RESOURCES/bin/Quneiform"

