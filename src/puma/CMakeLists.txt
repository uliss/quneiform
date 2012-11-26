add_definitions(-DBOOST_DATE_TIME_NO_LIB)

set(CF_WORKER cuneiform-worker)

set(CF_PUMA_SOURCE
    abstractrecognitionserver.cpp
    layoutblock.cpp
    localrecognitionserver.cpp
    pumaimpl.cpp
    recognitionfactory.cpp
    startprocess.cpp
)

if(CF_USE_SHMEM)
    list(APPEND CF_PUMA_SOURCE processrecognitionserver.cpp)
    set(BUILD_SHMEM_WORKER True)
    add_definitions(-DBUILD_PROCESS_RECOGNITION_SERVER)
else()
    set(BUILD_SHMEM_WORKER False)
endif()

if(UNIX)
    list(APPEND CF_PUMA_SOURCE processtimeoutkiller.cpp)
endif()

set(CF_PUMA_HEADERS
    abstractrecognitionserver.h
    puma.h
    process_exit_codes.h
)

set(CF_PUMA_LINK
    ${CF_COMMON}
    ${CF_CED}
    ${CF_CIMAGE}
    ${CF_CLINE}
    ${CF_CPAGE}
    ${CF_EXPORT}
    ${CF_MARKUP}
    ${CF_RBLOCK}
    ${CF_RCORRKEGL}
    ${CF_RFRMT}
    ${CF_RIMAGE}
    ${CF_RLINE}
    ${CF_RPIC}
    ${CF_RSELSTR}
    ${CF_RSL}
    ${CF_RSTUFF}
    ${CF_EXC}
    ${CF_RRECCOM}
    ${CF_RPSTR}
)

if(CF_USE_SHMEM)
    list(APPEND CF_PUMA_LINK ${CF_SHMEM} ${CF_LOAD})
endif()

if(CF_LIB_RT_HAS_SHMOPEN)
    list(APPEND CF_PUMA_LINK rt)
endif()

find_package(Threads)

if(CMAKE_USE_PTHREADS_INIT)
    if(NOT WIN32)
        list(APPEND CF_PUMA_LINK pthread)
    endif()
endif()

add_library(${CF_PUMA} ${CF_CUNEIFORM_LIBTYPE}
    ${CF_PUMA_SOURCE}
    ${CF_PUMA_HEADERS}
    ${SNP_SRC}
)

target_link_libraries(${CF_PUMA} ${CF_PUMA_LINK})
cf_install_library(${CF_PUMA})

if(BUILD_SHMEM_WORKER)
    add_executable(${CF_WORKER} worker.cpp)
    target_link_libraries(${CF_WORKER} ${CF_PUMA})

    if(WIN32)
        set(WORKER_INSTALL_DIR bin)
    else()
        set(WORKER_INSTALL_DIR lib/cuneiform)
    endif()

    install(TARGETS ${CF_WORKER}
        RUNTIME DESTINATION ${WORKER_INSTALL_DIR}
        COMPONENT Applications
    )
endif()

cf_install_headers(puma
    abstractrecognitionserver.h
    localrecognitionserver.h
    layoutblock.h
)

cf_cppcheck(${CF_PUMA})
