find_program(GZIP_TOOL
             NAMES gzip
             PATHS /bin
                   /usr/bin
                   /usr/local/bin)

if(NOT GZIP_TOOL)
    message(FATAL_ERROR "Unable to find 'gzip' program")
endif()
