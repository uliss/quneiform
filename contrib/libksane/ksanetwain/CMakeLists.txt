CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/../libksane/version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)


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

KDE4_ADD_LIBRARY(ksane SHARED ${ktwain_src})


TARGET_LINK_LIBRARIES(ksane ${KDE4_KDEUI_LIBS})

SET_TARGET_PROPERTIES(ksane PROPERTIES 
        VERSION ${KSANE_LIB_SO_VERSION_STRING} 
        SOVERSION ${KSANE_LIB_SO_CUR_VERSION}
        LINK_FLAGS -Wl,-subsystem,console
)

install(TARGETS ksane ${INSTALL_TARGETS_DEFAULT_ARGS} )

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../libksane/ksane.h
              ${CMAKE_CURRENT_SOURCE_DIR}/../libksane/libksane_export.h
              ${CMAKE_CURRENT_BINARY_DIR}/version.h
        DESTINATION ${INCLUDE_INSTALL_DIR}/libksane COMPONENT Devel)

set(INCLUDE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
get_target_property(LIB_OUT_NAME ksane LOCATION)
get_filename_component(LIB_OUT_NAME ${LIB_OUT_NAME} NAME)
set(LIBPATH ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})

configure_file(
    ../KSaneConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/KSaneConfig.cmake @ONLY
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/KSaneConfig.cmake
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/cmake/KSane
)
