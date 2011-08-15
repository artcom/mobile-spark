
project(AcMake)

# installation paths for our files
set(ACMAKE_MODULES_SUB lib/acmake/cmake)

# determine absolute build time locations of our files
set(ACMAKE_INSTALLED_MODULES_DIR ${CMAKE_INSTALL_PREFIX}/${ACMAKE_MODULES_SUB})

# generate the config script
configure_file(AcMakeConfig.cmake.in ${CMAKE_BINARY_DIR}/${ACMAKE_MODULES_SUB}/AcMakeConfig.cmake @ONLY)

# install config file
install(
    FILES ${CMAKE_BINARY_DIR}/${ACMAKE_MODULES_SUB}/AcMakeConfig.cmake
    DESTINATION ${ACMAKE_MODULES_SUB}
)

# install modules
install(
    DIRECTORY modules/
    DESTINATION ${ACMAKE_MODULES_SUB}
    FILES_MATCHING
        PATTERN "*.cmake"
        PATTERN ".svn" EXCLUDE
)

