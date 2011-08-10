set(SOURCES
TestBase.cpp
)

set(HEADERS
TestBase.h
)

include_directories (${LIBXML2_INCLUDE_DIRS})
add_library(testBase SHARED ${SOURCES} ${HEADERS})
#find_library(ANDROID_LOG_LIB log )

#find_path(Boost_INCLUDE_DIRS  boost/smart_ptr/shared_ptr.hpp HINTS $ENV{BOOST_HOME} /opt/local/include NO_CMAKE_FIND_ROOT_PATH)
include_directories (${Boost_INCLUDE_DIRS})

#find_path(CML_INCLUDE_DIR cml/cml.h ${PROJECT_SOURCE_DIR}/core/netsrc NO_CMAKE_FIND_ROOT_PATH)
include_directories (${CML_INCLUDE_DIR})

 # add imported target
#add_library(LIBXML2 STATIC IMPORTED)
# point the imported target at the real file
#set_property(TARGET LIBXML2 PROPERTY
#            IMPORTED_LOCATION ${LIBXML2_LIBRARY})
target_link_libraries(testBase base)
#target_link_libraries(testBase LIBXML2 ${ANDROID_LOG_LIB})
