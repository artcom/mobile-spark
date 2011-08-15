set(SOURCES
TestMasl.cpp
)

set(HEADERS
TestMasl.h
)

if($ENV{IS_TARGET_PLATFORM_ANDROID})
	add_library(testMasl SHARED ${SOURCES} ${HEADERS})
elseif($ENV{IS_TARGET_PLATFORM_IOS})
	add_library(testMasl STATIC ${SOURCES} ${HEADERS})
endif()

include_directories (${Boost_INCLUDE_DIRS})
include_directories (${LIBXML2_INCLUDE_DIRS})
include_directories (${CML_INCLUDE_DIR})

target_link_libraries(testMasl masl)
