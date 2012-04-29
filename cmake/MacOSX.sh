#!/bin/sh
# Author: Serge Poltavski 

QF_BUNDLE="`echo "$0" | sed -e 's/\/Contents\/MacOS\/Quneiform//'`"
QF_BUNDLE=${QF_BUNDLE%.sh}
QF_RESOURCES="$QF_BUNDLE/Contents/Resources"

echo "running $0"
echo "QF_BUNDLE: $QF_BUNDLE"

#export "DYLD_LIBRARY_PATH=$QF_RESOURCES/lib"
export "PATH=${QF_BUNDLE}/Contents/MacOS:${QF_BUNDLE}/Contents/Resources/bin:$PATH"
export "CF_DATADIR=$QF_RESOURCES/share/cuneiform"

echo "PATH: $PATH"

exec "${QF_BUNDLE}/Contents/MacOS/Quneiform" "$@"

