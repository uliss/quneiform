if(WITH_SANE)
    include(FindSane)

    if(SANE_FOUND)
        set(CF_SANE_SCANNER_SUPPORT ON)
    endif()
endif()
