set(SOURCES
Component.cpp
Container.cpp
Rectangle.cpp
SparkComponentFactory.cpp
Transform.cpp
Widget.cpp
Window.cpp
)

set(HEADERS
Component.h
Container.h
Rectangle.h
SparkComponentFactory.h
Transform.h
Widget.h
Window.h
)

add_library(spark STATIC ${SOURCES} ${HEADERS})

include_directories (${Boost_INCLUDE_DIRS})
include_directories (${LIBXML2_INCLUDE_DIRS})
include_directories(${LIBZIP_INCLUDE_DIR})
include_directories (${CML_INCLUDE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/core/src/base)
include_directories(${PROJECT_SOURCE_DIR}/core/src/android)

target_link_libraries(spark)

