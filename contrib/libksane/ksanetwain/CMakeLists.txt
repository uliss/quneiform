INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/../libksane
    ${CMAKE_CURRENT_BINARY_DIR}
)

IF( WIN32 )
    ADD_DEFINITIONS( -DMAKE_KSANE_LIB )
ENDIF( WIN32 )

set(ktwain_src
    twainiface.cpp
    ktwain_widget.cpp
)

set(ktwain_src_h
    twainiface.h
    ../libksane/ksane.h
)

qt4_wrap_cpp(ksane_MOC_SRCS ${ktwain_src_h})
ADD_LIBRARY(ksane STATIC ${ktwain_src} ${ksane_MOC_SRCS})

TARGET_LINK_LIBRARIES(ksane ${QT4_LIBRARIES})

set(INCLUDE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
get_target_property(LIB_OUT_NAME ksane LOCATION)
get_filename_component(LIB_OUT_NAME ${LIB_OUT_NAME} NAME)
set(LIBPATH ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
