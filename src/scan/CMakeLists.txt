set(CF_SCAN_SOURCE
    iscanner.cpp
    scanoptionvalue.cpp
    scanoptioninfo.cpp
    scanoption.cpp
)

set(CF_SCAN_LINK ${CF_COMMON})

if(CF_SANE_SCANNER_SUPPORT)
    if(SANE_FOUND)
        message(STATUS "Sane found at: " ${SANE_INCLUDE_DIR})
        include_directories(${SANE_INCLUDE_DIR})
        list(APPEND CF_SCAN_SOURCE sanescanner.cpp)
        list(APPEND CF_SCAN_LINK ${SANE_LIBRARY} ${CF_RDIB})
    endif()
endif()

add_library(${CF_SCAN} ${LIBTYPE} ${CF_SCAN_SOURCE})
target_link_libraries(${CF_SCAN} ${CF_SCAN_LINK})
cf_install_library(${CF_SCAN})
