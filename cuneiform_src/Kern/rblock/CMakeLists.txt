include_directories(BEFORE 
sources/h 
sources/main 
../hhh/tigerh
../hhh/tigerh/h)

add_library(rblock ${LIBTYPE}
${SNP_SRC}
sources/winc/_adaptive.cpp
sources/new_c/_debug.cpp
sources/main/_dll.cpp
sources/winc/_dlldummy.cpp
sources/winc/_graphics.cpp
sources/new_c/_loadroots.cpp
sources/main/_mymem.cpp
sources/new_c/_partlayout.cpp
sources/new_c/_partstring.cpp
sources/new_c/_pic_n_table.cpp
sources/main/_rblock.cpp
sources/c/compdata.cpp
sources/c/ltblglue.cpp
sources/c/ltblocks.cpp
sources/c/ltbreak.cpp
sources/c/lterrors.cpp
sources/c/ltexcomp.cpp
sources/c/lthand.cpp
sources/c/lthlundo.cpp
sources/c/lthyst.cpp
sources/c/ltibreak.cpp
sources/c/ltinclin.cpp
sources/c/ltmain.cpp
sources/c/ltpage.cpp
sources/c/ltremove.cpp
sources/c/ltroots.cpp
sources/c/ltseps.cpp
sources/c/ltsmart.cpp
sources/c/lttrees.cpp
sources/c/ltwstrip.cpp
sources/c/madebug.cpp
sources/c/msgerr.cpp
sources/c/qsort.cpp
sources/c/scalar.cpp
sources/c/seblocks.cpp
sources/c/seform.cpp
sources/c/sehbreak.cpp
sources/c/sehyst.cpp
sources/c/semain.cpp
sources/c/sestring.cpp
sources/c/sevbreak.cpp
)
set_property(TARGET rblock PROPERTY COMPILE_DEFINITIONS __RBLOCK__)

target_link_libraries(rblock cpage loc32 exc cstr cline rstr evn32)

install_dll(rblock)
