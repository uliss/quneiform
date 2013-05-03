
INCLUDE_DIRECTORIES(${SANE_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/options
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
)

SET(ksane_LIB_SRCS
    widgets/gamma_disp.cpp
    widgets/labeled_gamma.cpp
    widgets/labeled_checkbox.cpp
    widgets/labeled_entry.cpp
    widgets/labeled_combo.cpp
    widgets/labeled_fslider.cpp
    widgets/labeled_slider.cpp
    widgets/ksane_button.cpp
    widgets/ksane_option_widget.cpp
    ksane_viewer.cpp
    selectionitem.cpp
    ksane_device_dialog.cpp
    ksane_find_devices_thread.cpp
    ksane_widget.cpp
    ksane_scan_thread.cpp
    ksane_preview_thread.cpp
    ksane_widget_private.cpp
    splittercollapser.cpp
    ksane_auth.cpp
    options/ksane_option.cpp
    options/ksane_opt_button.cpp
    options/ksane_opt_checkbox.cpp
    options/ksane_opt_entry.cpp
    options/ksane_opt_gamma.cpp
    options/ksane_opt_slider.cpp
    options/ksane_opt_fslider.cpp
    options/ksane_opt_combo.cpp
    )

qt4_wrap_cpp(ksane_LIB_SRCS ${ksane_MOC_HDRS})

ADD_LIBRARY(ksane STATIC ${ksane_LIB_SRCS})

TARGET_LINK_LIBRARIES(ksane ${QT4_LIBRARIES}
                            ${SANE_LIBRARY}
                     )

SET_TARGET_PROPERTIES(ksane PROPERTIES VERSION ${KSANE_LIB_SO_VERSION_STRING} SOVERSION ${KSANE_LIB_SO_CUR_VERSION})

#INSTALL(TARGETS ksane ${INSTALL_TARGETS_DEFAULT_ARGS} )

#INSTALL(FILES ksane.h
#              libksane_export.h
#              ${CMAKE_CURRENT_BINARY_DIR}/version.h
#        DESTINATION ${INCLUDE_INSTALL_DIR}/libksane COMPONENT Devel)
#
#KDE4_INSTALL_ICONS(${ICON_INSTALL_DIR})


SET(INCLUDE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/include)
GET_TARGET_PROPERTY(LIB_OUT_NAME ksane LOCATION)
GET_FILENAME_COMPONENT(LIB_OUT_NAME ${LIB_OUT_NAME} NAME)
SET(LIBPATH ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})

#CONFIGURE_FILE(
#    ../KSaneConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/KSaneConfig.cmake @ONLY
#)

#INSTALL(
#    FILES ${CMAKE_CURRENT_BINARY_DIR}/KSaneConfig.cmake
#    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/cmake/KSane
#)
