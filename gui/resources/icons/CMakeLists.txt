add_subdirectory(oxygen)

if(WITH_THEME_MAC)
    add_subdirectory(mac)
endif()

if(WITH_THEME_FAENZA)
    add_subdirectory(faenza)
endif()

if(WITH_THEME_GNOME)
    add_subdirectory(gnome)
endif()

if(WITH_THEME_HUMAN)
    add_subdirectory(human)
endif()

if(WITH_THEME_SNOWISH)
    add_subdirectory(snowish)
endif()
