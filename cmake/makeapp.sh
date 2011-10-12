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
BUILDDIR=$3

export APP=$DESTDIR/Quneiform.app
export APPC=$APP/Contents
export APPM=$APPC/MacOS
export APPR=$APPC/Resources
export APPF=$APPC/Frameworks
export APP_EXE="${APPR}/bin/Quneiform"

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
cp "${SRCDIR}/cmake/MacOSX.sh" "${APPM}/Quneiform"
chmod +x "${APPM}/Quneiform"
cp "${BUILDDIR}/Quneiform" "${APP_EXE}"
cp "${BUILDDIR}/cuneiform-worker" "${APPR}/bin/cuneiform-worker"
echo "Copying translations..."
cp ${BUILDDIR}/gui/*.qm "${APPR}/share/cuneiform/locale"
echo "Copying Info.plist..."
cp "${BUILDDIR}/Info.plist" "${APPC}"

otool -L "${APP_EXE}" | tail -n +2 | tr -d '\t' | cut -f 1 -d ' ' | while read line; do
  case $line in
    ${BUILDDIR}/*)
      ourlib=`basename $line`
      echo "Copying $ourlib..."
      cp $line "${APPR}/lib" >/dev/null 2>&1
      install_name_tool -change $line @executable_path/../lib/$ourlib "$APP_EXE"
      install_name_tool -id @executable_path/../lib/$ourlib "${APPR}/lib/$ourlib"
      ;;
    /opt/local/lib/*)
      optlib=`basename $line`
      echo "Copying external $optlib..."
      cp $line "$APPF" >/dev/null 2>&1
      install_name_tool -change $line @executable_path/../../Frameworks/$optlib "$APP_EXE"
      install_name_tool -id @executable_path/../../Frameworks/$optlib "$APPF/$optlib"
      ;;
    *)
        echo $line
      ;;
    esac
done

rm -rf Quneiform.dmg "${DESTDIR}/Quneiform-${VERSION}.dmg"
cd $DESTDIR
macdeployqt Quneiform.app -dmg -no-plugins -verbose=0
mv Quneiform.dmg Quneiform-$VERSION.dmg

