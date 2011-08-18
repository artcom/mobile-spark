set(SOURCES
TestAnimation.cpp
)

set(HEADERS
TestAnimation.h
)

if($ENV{IS_TARGET_PLATFORM_ANDROID})
	add_library(testAnimation SHARED ${SOURCES} ${HEADERS})
elseif($ENV{IS_TARGET_PLATFORM_IOS})
	add_library(testAnimation STATIC ${SOURCES} ${HEADERS})
endif()

include_directories (${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/core/src)

target_link_libraries(testAnimation animation masl)
