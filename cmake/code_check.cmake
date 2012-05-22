include(CppcheckTargets)

function(cf_cppcheck name)
    if(TESTS)
        add_cppcheck(${name} UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE)
    endif()
endfunction()
