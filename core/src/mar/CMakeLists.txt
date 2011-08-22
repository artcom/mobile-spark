
file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

if($ENV{IS_TARGET_PLATFORM_ANDROID})
	add_library(mar SHARED ${SOURCES} ${HEADERS})
elseif($ENV{IS_TARGET_PLATFORM_IOS})
	add_library(mar STATIC ${SOURCES} ${HEADERS})
endif()

include_directories(${PROJECT_SOURCE_DIR}/core/src)
include_directories (${Boost_INCLUDE_DIRS})
include_directories (${CML_INCLUDE_DIR})
include_directories (${LIBXML2_INCLUDE_DIRS})

if($ENV{IS_TARGET_PLATFORM_ANDROID})
	find_library(OPENGL_ES_20 GLESv2)
	target_link_libraries(mar masl android png ${OPENGL_ES_20})
elseif($ENV{IS_TARGET_PLATFORM_IOS})
	target_link_libraries(mar masl)
    #TODO? add png?
	ADD_FRAMEWORK(mar OpenGLES)
endif()

