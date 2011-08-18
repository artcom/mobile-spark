file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

if($ENV{IS_TARGET_PLATFORM_ANDROID})
	add_library(animation SHARED ${SOURCES} ${HEADERS})
elseif($ENV{IS_TARGET_PLATFORM_IOS})
	add_library(animation STATIC ${SOURCES} ${HEADERS})
endif()

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/core/src)

target_link_libraries(animation masl)
