# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
# Copyright (C) 1993-2009, ART+COM AG Berlin, Germany <www.artcom.de>
#
# This file is part of the ART+COM CMake Library (acmake).
#
# It is distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)             
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#
#
# Macro to declare a library in the current acmake project
#
# __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
#

# Define a library in the current ACMake project.
#
# ac_add_library(
#    name path
#    HEADERS headerfile ...
#    [ SOURCES sourcefile ... ]
#    [ DEPENDS integrated_library ... ]
#    [ EXTERNS imported_package ... ]
#    [ STATIC SHARED IMPORTED]
# )
#
# The NAME is used in project exports and for the target name.
#
# The PATH defines where includes are installed within the installed include
# tree, which should be the path that will be used for actually including
# the files in a source file (i.e. "#include <foo/bar/bla.h>" => "foo/bar").
#
# HEADERS is a list of all public header files for the library.
#
# SOURCES is a list of source files for this library. If empty or not present
# all the library is treated as a header-only library
#
# DEPENDS can be used to reference libraries that are defined
# within this project or another cmake project.
#
# EXTERNS can be used to reference libraries that are defined
# outside the project (i.e. by a find package or pkg-config).
#
# STATIC libraries are supported too.
#
macro(ac_add_library LIBRARY_NAME LIBRARY_PATH)
    # put arguments into the THIS_LIBRARY namespace
    parse_arguments(THIS_LIBRARY
        "SOURCES;HEADERS;DEPENDS;EXTERNS"
        "STATIC;SHARED;IMPORTED;IMPORTED_LOCATION"
        ${ARGN}
    )
    
    # do the same manually for name and path
    set(THIS_LIBRARY_NAME "${LIBRARY_NAME}")
    set(THIS_LIBRARY_PATH "${LIBRARY_PATH}")

    if(THIS_LIBRARY_STATIC)
        set(THIS_LIBRARY_STYLE STATIC)
    elseif(THIS_LIBRARY_SHARED)
        set(THIS_LIBRARY_STYLE SHARED)
    endif(THIS_LIBRARY_STATIC)
    # define the library target
    if(NOT THIS_LIBRARY_IMPORTED)
        add_library(
            ${THIS_LIBRARY_NAME} ${THIS_LIBRARY_STYLE}
                ${THIS_LIBRARY_SOURCES}
                ${THIS_LIBRARY_HEADERS}
        )
    else()
        # add imported target
        add_library(${THIS_LIBRARY_NAME} STATIC IMPORTED)
        # point the imported target at the real file
        set_property(TARGET ${THIS_LIBRARY_NAME} PROPERTY
                    IMPORTED_LOCATION ${THIS_IMPORTED_LOCATION})
    endif()
    
    IF(IOS)
        #Prevents that Xcode automatically creates Release and Debug Folders for static libraries
        SET_TARGET_PROPERTIES(${THIS_LIBRARY_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
        SET_TARGET_PROPERTIES(${THIS_LIBRARY_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    ENDIF()
    
    # add global include and library paths
    _ac_add_global_paths(${THIS_LIBRARY_NAME})

    _ac_collect_dependency_paths(${THIS_LIBRARY_NAME} LIBRARY_DIRS INCLUDE_DIRS "${THIS_LIBRARY_DEPENDS}" "${THIS_LIBRARY_EXTERNS}")
    foreach(DIR ${INCLUDE_DIRS})
        _ac_add_include_path(${THIS_LIBRARY_NAME} "${DIR}" YES)
    endforeach(DIR)
    target_link_libraries(
        ${THIS_LIBRARY_NAME}
        ${LIBRARY_DIRS}
    )
    target_link_libraries(
        ${THIS_LIBRARY_NAME}
        ${THIS_LIBRARY_DEPENDS}
    )
    # Make sure we can access our includes without staging
    include_directories(../ ${CMAKE_CURRENT_BINARY_DIR}/../)

endmacro(ac_add_library)

