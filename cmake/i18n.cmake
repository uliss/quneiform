add_custom_target(i18n ""
    COMMAND cd "${CMAKE_SOURCE_DIR}/gui" && ./Messages.sh
    COMMAND cd "${CMAKE_SOURCE_DIR}/contrib/libksane/libksane" && ./Messages.sh
)
