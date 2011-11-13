include(CheckLibraryExists)
include(CheckFunctionExists)

if(NOT WIN32)
    list(APPEND CMAKE_SYSTEM_INCLUDE_PATH /usr/include/i386-linux-gnu /usr/include/x86_64-linux-gnu)
    message(STATUS "Searching for shmem library...")
    find_path(CF_SYS_IPC_PATH "sys/ipc.h")
    if(NOT CF_SYS_IPC_PATH)
        message(STATUS "sys/ipc.h - not found")
    else()
        find_path(CF_SYS_SHM_PATH "sys/shm.h")
        if(NOT CF_SYS_SHM_PATH)
            message(STATUS "sys/shm.h - not found.")
        else()
            set(USE_SYSTEMV_SHMEM True)
            check_function_exists("shm_open" CF_HAVE_SHMOPEN)
            if(NOT CF_HAVE_SHMOPEN)
                check_library_exists("rt" "shm_open" "" CF_LIB_RT_HAS_SHMOPEN)
            endif()
        endif() 
    endif()
endif()
