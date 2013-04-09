add_subdirectory(minizip)

set(CF_USE_MINIZIP True BOOL PARENT_SCOPE)

if(OCRTK OR TESTS)
    add_subdirectory(ocrtk)
endif()

if(SANE_FOUND)
    add_subdirectory(libsanewidget)
endif()
