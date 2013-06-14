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
VERSION=`cat "${SRCDIR}/version" | cut -d ' ' -f 3 | tr -d '"'`

"${SRCDIR}/cmake/mac_make_bundle.sh" "$DESTDIR" "$SRCDIR" "$BUILDDIR"

rm -rf Quneiform.dmg "${DESTDIR}/Quneiform-${VERSION}.dmg"

# DMG creation
cd $DESTDIR
rm -rf tmp.dmg

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
cp "$SRCDIR/gui/resources/macosx/dmg_background.png" "$DMG_BACKGROUND"
cp "$SRCDIR/gui/resources/macosx/disk_logo.icns" "$DMG_ICON"
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
