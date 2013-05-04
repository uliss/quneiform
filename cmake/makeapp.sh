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

rm -rf "${APP}"
mkdir -p "${APP}/Contents/MacOS"
mkdir -p "${APPR}"
mkdir -p "${APPF}"
mkdir -p "${APPR}/share/cuneiform/locale"
mkdir -p "${APPR}/lib"
mkdir -p "${APPR}/bin"

echo "Making locale folders..."
mkdir -p "${APPR}/en.lproj"
cp -R "${SRCDIR}/gui/resources/ru.lproj/" "${APPR}" 

VERSION=`cat "${SRCDIR}/version" | cut -d ' ' -f 3 | tr -d '"'`
echo "Build version: $VERSION"

echo "Copying bundle icon..."
cp "${SRCDIR}/gui/resources/cuneiform.icns" "${APPR}/Quneiform.icns"
cp "${SRCDIR}/gui/resources/packet.icns" "${APPR}/packet.icns"
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
    # source library path
    lib_path=$1
    # library name
    lib=`basename $lib_path`
    # bundle lib directory
    dest_lib_dir="${APPR}/lib"
    # bundle library path
    dest_lib_path="$dest_lib_dir/$lib"
    # load path reference in *.dylib
    dest_lib_load_path="@executable_path/../Resources/lib/${lib}"

    if [[ ! -f "$dest_lib_path" ]];
    then
        echo "Copying $lib..."
        cp $lib_path "$dest_lib_path" >/dev/null 2>&1

        # update executable binary reference
        install_name_tool -change $lib_path $dest_lib_load_path "$APP_EXE"
        install_name_tool -id $dest_lib_load_path "$dest_lib_path"

        # update other references in given library
        otool -L $dest_lib_path | tail -n +2 | tr -d '\t' | cut -f1 -d ' ' | while read line
        do
            case $line in
                $BUILDDIR/*)
                # dependency name
                ref=$(basename $line)
                install_name_tool -change $line "@executable_path/../Resources/lib/${ref}" $dest_lib_path
                ;;
            esac
        done
    fi
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
rm -rf tmp.dmg

echo "Creating Quneiform.app bundle..."
macdeployqt Quneiform.app -verbose=1

echo "Creating DMG..."
VOLNAME="Quneiform OCR"
hdiutil create -srcfolder Quneiform.app -format UDRW -volname "$VOLNAME" -o tmp.dmg -scrub

echo "Mount DMG..."
device=$(hdiutil attach -readwrite -noverify -noautoopen tmp.dmg | egrep '^/dev/' | sed 1q | awk '{print $1}')
mountdir=$(hdiutil info | grep $device | grep Volumes | cut -f 3)
echo "Mount $device at $mountdir"
title=$(basename $mountdir)

sleep 1

echo "Adding logo and background..."
DMG_BACKGROUND="$mountdir/logo.png"
DMG_ICON="$mountdir/.VolumeIcon.icns"
cp "$SRCDIR/gui/resources/dmg_background.png" "$DMG_BACKGROUND"
cp "$SRCDIR/gui/resources/disk_logo.icns" "$DMG_ICON"
SetFile -c icnC "$DMG_ICON"
SetFile -a C "$mountdir"

echo "Adding license..."
cp "$SRCDIR/COPYING.txt" "$mountdir/License.txt"

echo "Making nice look..."
echo '
on run argv
   tell application "Finder"
     tell disk "'$VOLNAME'"
           open
           set current view of container window to icon view
           set toolbar visible of container window to false
           set statusbar visible of container window to false
           set the bounds of container window to {400, 100, 900, 500}
           set theViewOptions to the icon view options of container window
           set arrangement of theViewOptions to not arranged
           set icon size of theViewOptions to 72
           make new alias file at container window to POSIX file "/Applications" with properties {name:"Applications"}
           set position of item "Quneiform" of container window to {100, 100}
           set position of item "Applications" of container window to {400, 100}
           set position of item "License.txt" of container window to {100, 250}
           set background picture of theViewOptions to file "logo.png"
           close
           delay 1
           open
           delay 1
           close
     end tell
   end tell
end run
' | osascript

# hide background file
SetFile -a V "$DMG_BACKGROUND"

sync
sync

chmod -Rf go-w "/Volumes/$VOLNAME"
hdiutil detach ${device}

sleep 1

echo "Compressing DMG..."

hdiutil convert tmp.dmg -format UDZO -imagekey zlib-level=9 -o "Quneiform-${VERSION}.dmg"
rm -rf tmp.dmg
