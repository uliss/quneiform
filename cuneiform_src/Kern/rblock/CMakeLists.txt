include_directories(BEFORE 
sources/h 
sources/main 
../hhh/tigerh
../hhh/tigerh/h)

add_library(rblock ${LIBTYPE}
${SNP_SRC}
_adaptive.c
_debug.c
_dll.cpp
_dlldummy.c
_graphics.c
_loadroots.c
_mymem.cpp
_partlayout.c
_partstring.c
_pic_n_table.c
_rblock.cpp
compdata.c
ltblglue.c
ltblocks.c
ltbreak.c
lterrors.c
ltexcomp.c
lthand.c
lthlundo.c
lthyst.c
ltibreak.c
ltinclin.c
ltmain.c
ltpage.c
ltremove.c
ltroots.c
ltseps.c
ltsmart.c
lttrees.c
ltwstrip.c
madebug.c
msgerr.c
qsort.c
scalar.c
seblocks.c
seform.c
sehbreak.c
sehyst.c
semain.c
sestring.c
sevbreak.c
)
set_property(TARGET rblock PROPERTY COMPILE_DEFINITIONS __RBLOCK__)

target_link_libraries(rblock cpage loc32 exc cstr cline rstr evn32)

install_dll(rblock)
