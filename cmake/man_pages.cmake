if(UNIX)
    # Search for gzip program
    include(FindGZIP)

    set(cuneiform_man_src cuneiform_en.1 cuneiform_ru.1)
    set(cuneiform_man_gz cuneiform_en.1.gz cuneiform_ru.1.gz)

    set(CF_DATE "")
    execute_process(COMMAND "date" "+%Y-%m-%d" OUTPUT_VARIABLE CF_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)

    foreach(man ${cuneiform_man_src})
        configure_file(${CMAKE_SOURCE_DIR}/doc/${man} ${CMAKE_BINARY_DIR}/man/${man})
    endforeach()

    # Compression of the manuals
    foreach(man ${cuneiform_man_src})
        add_custom_command(OUTPUT ${man}.gz
                     COMMAND ${GZIP_TOOL} -c ${CMAKE_BINARY_DIR}/man/${man} > ${CMAKE_BINARY_DIR}/man/${man}.gz
                     DEPENDS ${CMAKE_BINARY_DIR}/man/${man}
                     COMMENT "Building ${man}.gz")
    endforeach()

    # Add the manual generation in the global rules
    add_custom_target(man ALL
                     DEPENDS ${cuneiform_man_gz})

    # Installation of the manuals
    install(FILES ${CMAKE_BINARY_DIR}/man/cuneiform_ru.1.gz
        DESTINATION "share/man/ru/man1"
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME cuneiform.1.gz)
    install(FILES ${CMAKE_BINARY_DIR}/man/cuneiform_en.1.gz
        DESTINATION "share/man/man1"
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME cuneiform.1.gz)
endif()
