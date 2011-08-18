
file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

if($ENV{IS_TARGET_PLATFORM_ANDROID})
	add_library(spark SHARED ${SOURCES} ${HEADERS})
elseif($ENV{IS_TARGET_PLATFORM_IOS})
	add_library(spark STATIC ${SOURCES} ${HEADERS})
endif()

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LIBXML2_INCLUDE_DIRS})
include_directories(${CML_INCLUDE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/core/src)

if($ENV{IS_TARGET_PLATFORM_ANDROID})
	include_directories(${LIBZIP_INCLUDE_DIR})
	target_link_libraries(spark animation android mar)
	
elseif($ENV{IS_TARGET_PLATFORM_IOS})

	target_link_libraries(spark mar)
endif()
