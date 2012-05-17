set(CF_ALPHABETS cuneiform-alphabets)
set(CF_ALPHABETS_LINK
    cuneiform-compat
    cuneiform-common
)
set(CF_ALPHABETS_SOURCE
    alphabetfactory.cpp
    alphabet.cpp
    baltic.cpp
    belarusian.cpp
    bulgar.cpp
    croatian.cpp
    czech.cpp
    danish.cpp
    digits.cpp
    dutch.cpp
    easteurope.cpp
    english.cpp
    estonian.cpp
    french.cpp
    german.cpp
    hungar.cpp
    italian.cpp
    kazakh.cpp
    kazakhenglish.cpp
    latvian.cpp
    lithuanian.cpp
    polish.cpp
    portuguese.cpp
    roman.cpp
    russian.cpp
    russianenglish.cpp
    serbian.cpp
    slovenian.cpp
    spanish.cpp
    swedish.cpp
    turkish.cpp
    ukrainian.cpp
    uzbek.cpp
)

add_library(${CF_ALPHABETS} ${LIBTYPE} ${CF_ALPHABETS_SOURCE})
target_link_libraries(${CF_ALPHABETS} ${CF_ALPHABETS_LINK})

cf_install_library(${CF_ALPHABETS})
cf_install_headers(alphabets alphabetfactory.h alphabet.h)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-alphabets UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR)
endif()
