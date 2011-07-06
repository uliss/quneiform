add_subdirectory(minizip)

set(CF_USE_MINIZIP True BOOL PARENT_SCOPE)

if(OCRTK)
    add_subdirectory(ocrtk)
endif()
