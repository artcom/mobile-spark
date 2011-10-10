message("....................cmake path of android curl. this cmake is never reached.......................")
find_path(LIBCURL_INCLUDE_DIRS NAMES curl/curl.h PATHS ${PROJECT_SOURCE_DIR}/core/netsrc/libcurl/android/ NO_CMAKE_FIND_ROOT_PATH)
find_library(LIBCURL libcurl.a ${PROJECT_SOURCE_DIR}/core/netsrc/libcurl/android/ NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH )
include_directories (${LIBCURL_INCLUDE_DIRS})
