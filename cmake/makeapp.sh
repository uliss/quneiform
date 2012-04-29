#!/bin/bash

if [ "$#" -lt 3 ]
then
  echo "Usage:  makeapp.sh <DEST_DIR> <SRC_DIR> <BUILD_DIR>"
  echo " "
  echo "DEST_DIR is where the output files are created."
  echo "SRC_DIR is where the quneiform source files are."
  echo "BUILD_DIR is the working directory where dependency libs are built."
  echo ""
  exit 0
fi

DESTDIR=$1
SRCDIR=$2
BUILDDIR=`echo "$3" | sed -e "s/\/*$//" `

export APP=$DESTDIR/Quneiform.app
export APPC=$APP/Contents
export APPM=$APPC/MacOS
export APPR=$APPC/Resources
export APPF=$APPC/Frameworks
export APP_EXE="${APPM}/Quneiform"
export APP_WORKER="${APPM}/cuneiform-worker"

rm -rf "$APP"
mkdir -p "${APP}/Contents/MacOS"
mkdir -p "${APPR}"
mkdir -p "${APPF}"
mkdir -p "${APPR}/share/cuneiform/locale"
mkdir -p "${APPR}/lib"
mkdir -p "${APPR}/bin"

VERSION=`cat "${SRCDIR}/version" | cut -d ' ' -f 3 | tr -d '"'`
echo "Build version: $VERSION"

echo "Copying bundle icon..."
cp "${SRCDIR}/gui/resources/cuneiform.icns" "${APPR}/Quneiform.icns"
echo "Copying datafiles..."
cp ${SRCDIR}/datafiles/*.dat "${APPR}/share/cuneiform"
echo "Copying executables..."

# copy executables
cp "${SRCDIR}/cmake/MacOSX.sh" "${APPM}/Quneiform.sh"
chmod +x "${APPM}/Quneiform.sh"

cp "${BUILDDIR}/Quneiform" "${APPM}/Quneiform"
cp "${BUILDDIR}/cuneiform-worker" "${APPM}/cuneiform-worker"

echo "Copying translations..."
cp ${BUILDDIR}/gui/*.qm "${APPR}"
echo "Copying Info.plist..."
cp "${BUILDDIR}/Info.plist" "${APPC}"

function install_our {
    lib_path=$1
    lib=`basename $lib_path`
    dest_lib_dir="${APPR}/lib"
    dest_lib_path="$dest_lib_dir/$lib"
    dest_lib_load_path="@loader_path/../Resources/lib/$lib"

    echo "Copying $lib into $dest_lib_dir"
    cp $lib_path "$dest_lib_dir" >/dev/null 2>&1

    # update executable binary
    install_name_tool -change $lib_path $dest_lib_load_path "$APP_EXE"
    #install_name_tool -change $lib_path $dest_lib_load_path "$APP_WORKER"
    install_name_tool -id $dest_lib_load_path "$dest_lib_path"
}

otool -L "${APP_EXE}" | tail -n +2 | tr -d '\t' | cut -f 1 -d ' ' | while read line; do
  case $line in
    ${BUILDDIR}/*)
      install_our $line
      ;;
    *)
        #echo "INFO: skipping $line"
      ;;
    esac
done

rm -rf Quneiform.dmg "${DESTDIR}/Quneiform-${VERSION}.dmg"

# DMG creation
cd $DESTDIR

echo "Creating DMG..."
macdeployqt Quneiform.app -dmg -no-plugins -verbose=2
mv Quneiform.dmg Quneiform-$VERSION.dmg

