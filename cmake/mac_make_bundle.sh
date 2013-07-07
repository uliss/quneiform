#!/bin/bash

if [ "$#" -lt 3 ]
then
  echo "Usage:  mac_make_bundle.sh <DEST_DIR> <SRC_DIR> <BUILD_DIR>"
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
export APPH=$APPR/Quneiform.help
export APPHC=$APPH/Contents
export APPHR=$APPHC/Resources
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
cp -R "${SRCDIR}/gui/resources/macosx/ru.lproj/" "${APPR}" 
cp /Developer/Applications/Qt/translations/qt_*.qm "${APPR}"

VERSION=`cat "${SRCDIR}/version" | cut -d ' ' -f 3 | tr -d '"'`
echo "Build version: $VERSION"

echo "Copying bundle icon..."
cp "${SRCDIR}/gui/resources/macosx/cuneiform.icns" "${APPR}/Quneiform.icns"
cp "${SRCDIR}/gui/resources/macosx/packet.icns" "${APPR}/packet.icns"
echo "Copying documentation..."
cp -R "${SRCDIR}/gui/resources/macosx/Quneiform.help" "${APPR}"
cp "${SRCDIR}/gui/resources/cuneiform_logo_32.png" "${APPR}/Quneiform.help/Contents/Resources/shrd/cuneiform.png"
echo "Generating help indexes..."
hiutil -Cagf "$APPHR/English.lproj/Quneiform.helpindex" "$APPHR/English.lproj"
hiutil -Cagf "$APPHR/ru.lproj/Quneiform.helpindex" "$APPHR/ru.lproj"
echo "Copying datafiles..."
cp ${SRCDIR}/datafiles/*.dat "${APPR}/share/cuneiform"

echo "Copying executables..."
cp "${BUILDDIR}/Quneiform" "${APPM}/Quneiform"
cp "${BUILDDIR}/cuneiform-worker" "${APPM}/cuneiform-worker"

echo "Copying translations..."
cp ${BUILDDIR}/gui/*.qm "${APPR}"
echo "Copying Info.plist..."
cp "${BUILDDIR}/Info.plist" "${APPC}"
echo "Copying nib files..."
cp "${BUILDDIR}/ScanWindow.nib" "${APPR}"

# extract debug symbols
echo "Extracting debug symbols..."
dsymutil "$APPM/Quneiform" -o Quneiform.app.dSYM

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

echo "Fixing cuneiform-worker linking paths..."
otool -L "${APP_WORKER}" | tail -n +2 | tr -d '\t' | cut -f 1 -d ' ' | while read line; do
    case $line in
        *libcuneiform*)
            ref=$(basename $line)
            echo "    fixing project lib: $ref"
            install_name_tool -change $line "@executable_path/../Resources/lib/${ref}" "${APP_WORKER}"
        ;;
        *libpoppler* | *libtiff* | *libiconv* | *libboost* | *libz*)
            ref=$(basename $line)
            echo "    fixing required lib: $ref"
            install_name_tool -change $line "@executable_path/../Frameworks/${ref}" "${APP_WORKER}"
        ;;
        QtCore* | QtGui*)
            ref=$(basename $line)
            echo "    fixing Qt framework link: $ref"
            install_name_tool -change $line "@executable_path/../Frameworks/${line}" "${APP_WORKER}"
        ;;
        *)
            echo "    skipping $line"
        ;;
    esac
done

cd $DESTDIR

echo "Creating Quneiform.app bundle..."
macdeployqt Quneiform.app -verbose=1

echo "Removing unused Qt frameworks: declarative, script, network, sql, xmlpatterns"
rm -rf "$APPF/QtDeclarative.framework"
rm -rf "$APPF/QtScript.framework"
rm -rf "$APPF/QtNetwork.framework"
rm -rf "$APPF/QtSql.framework"
rm -rf "$APPF/QtXmlPatterns.framework"

echo "Removing unused Qt plugins: accessible, bearer, codecs, qmltooling"
rm -rf "$APPC/PlugIns/accessible"
rm -rf "$APPC/PlugIns/bearer"
rm -rf "$APPC/PlugIns/codecs"
rm -rf "$APPC/PlugIns/qmltooling"
