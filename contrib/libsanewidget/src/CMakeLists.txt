# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
#   set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

# the variable "sanewidget_SRCS" contains all .cpp files of this project
SET(sanewidget_SRCS
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

SET(sanewidget_MOC_HDRS
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

# After this call, sanewidget_MOC_SRCS = moc_Class1.cxx moc_Class2.cxx moc_Class3.cxx.
QT4_WRAP_CPP(sanewidget_MOC_SRCS ${sanewidget_MOC_HDRS})

ADD_LIBRARY(sanewidget SHARED ${sanewidget_SRCS} ${sanewidget_MOC_SRCS})


# link the "sanewidget" target against the Qt libraries. which libraries exactly,
# is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable.
TARGET_LINK_LIBRARIES(sanewidget ${QT_LIBRARIES} ${SANE_LIBRARY})

# set library version
SET_TARGET_PROPERTIES(sanewidget PROPERTIES  VERSION 0.2.0 SOVERSION 0)

#INSTALL(TARGETS sanewidget LIBRARY DESTINATION lib )
#INSTALL(FILES sane_widget.h DESTINATION include )
