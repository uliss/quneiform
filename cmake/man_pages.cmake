if(UNIX)
    # Search for gzip program
    include(FindGZIP)

    set(cuneiform_man_src cuneiform_en.1 cuneiform_ru.1)
    set(cuneiform_man_gz cuneiform_en.1.gz cuneiform_ru.1.gz)

    set(CF_DATE "")
    execute_process(COMMAND "date" "+%Y-%m-%d" OUTPUT_VARIABLE CF_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)

    set(MAN_LOCAL ${CMAKE_BINARY_DIR}/man)

    foreach(man ${cuneiform_man_src})
        configure_file(${CMAKE_SOURCE_DIR}/doc/${man} ${MAN_LOCAL}/${man})
    endforeach()

    # Compression of the manuals
    foreach(man ${cuneiform_man_src})
        add_custom_command(OUTPUT ${MAN_LOCAL}/${man}.gz
                     COMMAND ${GZIP_TOOL} -c ${MAN_LOCAL}/${man} > ${MAN_LOCAL}/${man}.gz
                     DEPENDS ${MAN_LOCAL}/${man}
                     COMMENT "Building ${man}.gz")
    endforeach()

    # Add the manual generation in the global rules
    add_custom_target(man ALL
                     DEPENDS ${MAN_LOCAL}/cuneiform_en.1.gz
                     DEPENDS ${MAN_LOCAL}/cuneiform_ru.1.gz)

    # Installation of the manuals
    install(FILES ${MAN_LOCAL}/cuneiform_ru.1.gz
        DESTINATION "share/man/ru/man1"
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME cuneiform.1.gz)
    install(FILES ${MAN_LOCAL}/cuneiform_en.1.gz
        DESTINATION "share/man/man1"
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ RENAME cuneiform.1.gz)
endif()
