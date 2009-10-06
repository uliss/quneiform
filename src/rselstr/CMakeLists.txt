include_directories(BEFORE
sources/src/resourse
../hhh/tigerh/h/
../hhh/tigerh/
sources/src/h
sources/src/chstr/h
)

add_library(rselstr ${LIBTYPE}
${SNP_SRC}
sources/src/win_c/_adaptive.cpp
sources/src/win_c/_dlldummy.cpp
sources/src/cpp/_loadroots.cpp
sources/src/chstr/cutstr/chstr_cutstr.cpp
sources/src/chstr/list/chstr_lhstr.cpp
sources/src/cpp/compdata.cpp
sources/src/cpp/cutcomp.cpp
sources/src/cpp/cutstr.cpp
sources/src/win_c/graphics.cpp
sources/src/cpp/ltblocks.cpp
sources/src/cpp/ltbreak.cpp
sources/src/cpp/lterrors.cpp
sources/src/cpp/lthyst.cpp
sources/src/cpp/ltinclin.cpp
sources/src/cpp/ltmain.cpp
sources/src/cpp/ltroots.cpp
sources/src/chstr/cont/puttocont.cpp
sources/src/chstr/addtocstr/puttocstr.cpp
sources/src/cpp/qsort.cpp
sources/src/chstr/rotate/rotate.cpp
sources/src/rselstrmain/rselstrbase.cpp
sources/src/rselstrmain/rselstrmain.cpp
sources/src/cpp/scalar.cpp
sources/src/cpp/seblocks.cpp
sources/src/cpp/seform.cpp
sources/src/cpp/sehbreak.cpp
sources/src/cpp/sehyst.cpp
sources/src/cpp/semain.cpp
sources/src/cpp/sestring.cpp
sources/src/cpp/sevbreak.cpp
sources/src/chstr/vertical/testforvertical.cpp
)
set_property(TARGET rselstr PROPERTY COMPILE_DEFINITIONS __RSELSTR__)

target_link_libraries(rselstr
cimage
cpage
loc32
rimage
exc  
cline
cstr
rcutp
)

install_dll(rselstr)
