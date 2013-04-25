# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

# the variable "sanewidget_SRCS" contains all .cpp files of this project
set(sanewidget_SRCS
    gamma_disp.cpp
    labeled_gamma.cpp
    labeled_checkbox.cpp
    labeled_entry.cpp
    labeled_separator.cpp
    preview_image.cpp
    sane_option.cpp
    labeled_combo.cpp
    labeled_fslider.cpp
    labeled_slider.cpp
    preview_area.cpp
    radio_select.cpp
    sane_widget.cpp
)

set(sanewidget_MOC_HDRS
    gamma_disp.h
    labeled_gamma.h
    labeled_checkbox.h
    labeled_entry.h
    labeled_separator.h
    preview_image.h
    sane_option.h
    labeled_combo.h
    labeled_fslider.h
    labeled_slider.h
    preview_area.h
    radio_select.h
    sane_widget.h
)

set(SANEWIDGET_TRANSLATIONS_TS i18n/sanewidget_ru.ts)

qt4_create_translation(SANEWIDGET_TRANSLATIONS_QM
    ${sanewidget_SRCS}
    ${sanewidget_MOC_HDRS}
    ${SANEWIDGET_TRANSLATIONS_TS}
    OPTIONS -locations none
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/i18n.qrc
               ${CMAKE_CURRENT_BINARY_DIR}/sanewidget_i18n.qrc
               COPYONLY)

qt4_add_resources(SANEWIDGET_QRC ${CMAKE_CURRENT_BINARY_DIR}/sanewidget_i18n.qrc)

# After this call, sanewidget_MOC_SRCS = moc_Class1.cxx moc_Class2.cxx moc_Class3.cxx.
QT4_WRAP_CPP(sanewidget_MOC_SRCS ${sanewidget_MOC_HDRS})

ADD_LIBRARY(sanewidget STATIC
    ${sanewidget_SRCS}
    ${sanewidget_MOC_SRCS}
    ${SANEWIDGET_QRC}
    ${SANEWIDGET_TRANSLATIONS_QM})

# link the "sanewidget" target against the Qt libraries. which libraries exactly,
# is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable.
TARGET_LINK_LIBRARIES(sanewidget ${QT_LIBRARIES} ${SANE_LIBRARY} ${SANEWIDGET_QRC})
