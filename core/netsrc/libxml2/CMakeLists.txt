message("libxml2 cmake; this cmake-file is NEVER reached")
find_path(LIBXML2_INCLUDE_DIRS NAMES libxml/xpath.h PATHS ${PROJECT_SOURCE_DIR}/core/netsrc/libxml2/includes/ NO_CMAKE_FIND_ROOT_PATH)
find_library(LIBXML2_LIBRARY libxml2.a ${PROJECT_SOURCE_DIR}/core/netsrc/libxml2/ NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH )
include_directories (${LIBXML2_INCLUDE_DIRS})
