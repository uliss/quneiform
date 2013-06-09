PROJECT(libksane)

find_package(Sane REQUIRED)
include(FindLibintl.cmake)

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project
include(${QT_USE_FILE})

add_definitions(${QT_DEFINITIONS})
add_definitions(-DSANE_GETTEXT_DIR="${SANE_INCLUDE_DIR}/../share/locale")


set(KSANE_LIB_VERSION_STRING "${KSANE_LIB_MAJOR_VERSION}.${KSANE_LIB_MINOR_VERSION}.${KSANE_LIB_PATCH_VERSION}${KSANE_LIB_SUFFIX_VERSION}")
set(KSANE_LIB_VERSION_ID "0x0${KSANE_LIB_MAJOR_VERSION}0${KSANE_LIB_MINOR_VERSION}0${KSANE_LIB_PATCH_VERSION}")
set(KSANE_LIB_SO_VERSION_STRING "${KSANE_LIB_SO_CUR_VERSION}.${KSANE_LIB_SO_REV_VERSION}.${KSANE_LIB_SO_AGE_VERSION}")

# To prevent warnings from M$ compiler
if(WIN32 AND MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_ATL_SECURE_NO_WARNINGS)
    add_definitions(-D_AFX_SECURE_NO_WARNINGS)
endif()

if(SANE_FOUND)
    ADD_SUBDIRECTORY(libksane)
else()
    ADD_SUBDIRECTORY(ksanetwain)
endif()
