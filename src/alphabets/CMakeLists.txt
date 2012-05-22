set(CF_ALPHABETS_LINK
    ${CF_COMPAT}
    ${CF_COMMON}
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

set(CF_ALPHABETS_API alphabetfactory.h alphabet.h)

add_library(${CF_ALPHABETS} ${LIBTYPE} ${CF_ALPHABETS_SOURCE})
target_link_libraries(${CF_ALPHABETS} ${CF_ALPHABETS_LINK})

cf_install_library(${CF_ALPHABETS})
cf_install_headers(alphabets ${CF_ALPHABETS_API})
cf_cppcheck(${CF_ALPHABETS})
