file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

if($ENV{IS_TARGET_PLATFORM_ANDROID})
	add_library(masl SHARED ${SOURCES} ${HEADERS})
elseif($ENV{IS_TARGET_PLATFORM_IOS})
	add_library(masl STATIC ${SOURCES} ${HEADERS})
endif()

find_path(CML_INCLUDE_DIR cml/cml.h ${PROJECT_SOURCE_DIR}/core/netsrc NO_CMAKE_FIND_ROOT_PATH)
find_path(Boost_INCLUDE_DIRS  boost/smart_ptr/shared_ptr.hpp HINTS ${PROJECT_SOURCE_DIR}/core/netsrc $ENV{BOOST_HOME} /opt/local/include NO_CMAKE_FIND_ROOT_PATH)

#only for android
if($ENV{IS_TARGET_PLATFORM_ANDROID})
	find_library(ANDROID_LOG_LIB log )
	find_path(LIBZIP_INCLUDE_DIR libzip/zip.h ${PROJECT_SOURCE_DIR}/core/netsrc/ NO_CMAKE_FIND_ROOT_PATH)
	
	include_directories(${LIBZIP_INCLUDE_DIR})
elseif($ENV{IS_TARGET_PLATFORM_IOS})
	find_path(LIBXML2_INCLUDE_DIRS NAMES libxml/parser.h PATHS ${CMAKE_OSX_SYSROOT}/usr/include/libxml2 use NO_CMAKE_FIND_ROOT_PATH)
	find_library(LIBXML2_LIBRARY libxml2.dylib ${CMAKE_OSX_SYSROOT}/usr/lib/ NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH )
endif()

include_directories(${LIBXML2_INCLUDE_DIRS})
include_directories(${CML_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

# add imported target
add_library(LIBXML2 STATIC IMPORTED)
# point the imported target at the real file
set_property(TARGET LIBXML2 PROPERTY
            IMPORTED_LOCATION ${LIBXML2_LIBRARY})

if($ENV{IS_TARGET_PLATFORM_ANDROID})
	target_link_libraries(masl LIBXML2 ${ANDROID_LOG_LIB} )
elseif($ENV{IS_TARGET_PLATFORM_IOS})
	target_link_libraries(masl LIBXML2)
endif()

add_subdirectory(Tests)
