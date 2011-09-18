# - Run cppcheck on c++ source files as a custom target and a test
#
#  include(CppcheckTargets)
#  add_cppcheck(<target-name> [UNUSED_FUNCTIONS] [STYLE] [POSSIBLE_ERROR] [FAIL_ON_WARNINGS]) -
#    Create a target to check a target's sources with cppcheck and the indicated options
#
# Requires these CMake modules:
#  Findcppcheck
#
# Requires CMake 2.6 or newer (uses the 'function' command)
#
# Original Author:
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryand.net>
# http://academic.cleardefinition.com
# Iowa State University HCI Graduate Program/VRAC
#
# Copyright Iowa State University 2009-2010.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

if(__add_cppcheck)
    return()
endif()

set(__add_cppcheck YES)

if(NOT CPPCHECK_FOUND)
    find_package(cppcheck)
endif()

if(CPPCHECK_FOUND)
    if(NOT TARGET all_cppcheck)
        add_custom_target(all_cppcheck)
        set_target_properties(all_cppcheck PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endif()
endif()

function(add_cppcheck _name)
    if(NOT TARGET ${_name})
        message(FATAL_ERROR "add_cppcheck given a target name that does not exist: '${_name}'!")
    endif()

    if(CPPCHECK_FOUND)
        set(_cppcheck_args)

        list(APPEND _cppcheck_args ${CPPCHECK_INCLUDEPATH_ARG} ${CMAKE_SOURCE_DIR}/src/include)
        list(APPEND _cppcheck_args ${CPPCHECK_INCLUDEPATH_ARG} ${CMAKE_SOURCE_DIR}/src/h)
        list(APPEND _cppcheck_args ${CPPCHECK_INCLUDEPATH_ARG} ${CMAKE_SOURCE_DIR}/src)
        list(APPEND _cppcheck_args ${CPPCHECK_INCLUDEPATH_ARG} ${CMAKE_SOURCE_DIR}/contrib)
        list(APPEND _cppcheck_args ${CPPCHECK_INCLUDEPATH_ARG} ${CMAKE_BINARY_DIR}/src/include)
        list(APPEND _cppcheck_args ${CPPCHECK_INCLUDEPATH_ARG} ${CMAKE_SOURCE_DIR}/src/hrstr)
        list(APPEND _cppcheck_args ${CPPCHECK_INCLUDEPATH_ARG} ${CMAKE_SOURCE_DIR}/src/hdebug)
        list(APPEND _cppcheck_args ${CPPCHECK_INCLUDEPATH_ARG} ${CMAKE_SOURCE_DIR}/src/hhh)
        list(APPEND _cppcheck_args ${CPPCHECK_INCLUDEPATH_ARG} ${CMAKE_SOURCE_DIR}/src/hh)

        list(FIND ARGN UNUSED_FUNCTIONS _unused_func)
        if("${_unused_func}" GREATER "-1")
            list(APPEND _cppcheck_args ${CPPCHECK_UNUSEDFUNC_ARG})
        endif()

        list(FIND ARGN STYLE _style)
        if("${_style}" GREATER "-1")
            list(APPEND _cppcheck_args ${CPPCHECK_STYLE_ARG})
        endif()

        list(FIND ARGN POSSIBLE_ERROR _poss_err)
        if("${_poss_err}" GREATER "-1")
            list(APPEND _cppcheck_args ${CPPCHECK_POSSIBLEERROR_ARG})
        endif()

        list(FIND _input FAIL_ON_WARNINGS _fail_on_warn)
        if("${_fail_on_warn}" GREATER "-1")
            list(APPEND
                CPPCHECK_FAIL_REGULAR_EXPRESSION
                ${CPPCHECK_WARN_REGULAR_EXPRESSION})
            list(REMOVE_AT _input ${_unused_func})
        endif()

        set(_files)
        get_target_property(_cppcheck_sources "${_name}" SOURCES)

        foreach(_source ${_cppcheck_sources})
            get_source_file_property(_cppcheck_lang "${_source}" LANGUAGE)
            get_source_file_property(_cppcheck_loc "${_source}" LOCATION)
            if("${_cppcheck_lang}" MATCHES "CXX")
                list(APPEND _files "${_cppcheck_loc}")
            endif()
        endforeach()

        add_custom_target(
            ${_name}-cppcheck
            COMMAND
            "${CPPCHECK_EXECUTABLE}"
            ${CPPCHECK_TEMPLATE_ARG}
            ${CPPCHECK_QUIET_ARG}
            "--force"
            ${_cppcheck_args}
            ${_files})

        message(STATUS "${_name}-cppcheck target added.")
    endif()
endfunction()
