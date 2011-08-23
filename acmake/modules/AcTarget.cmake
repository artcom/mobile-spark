# utility function for extending FLAGS-like target properties
function(_ac_prepend_target_flag TARGET PROPERTY VALUE)
    get_property(FLAGS TARGET ${TARGET} PROPERTY ${PROPERTY})
    set_property(TARGET ${TARGET} PROPERTY ${PROPERTY} "${VALUE} ${FLAGS}")
endfunction(_ac_prepend_target_flag TARGET PROPERTY VALUE)

# add include path to a target for its own use
# PROPAGATE indicates if this path should be used by dependents
function(_ac_add_include_path TARGET PATH PROPAGATE)
    _ac_prepend_target_flag(${TARGET} COMPILE_FLAGS "-I\"${PATH}\"")
    if(PROPAGATE)
        prepend_global(${TARGET}_INCLUDE_DIRS "${PATH}")
    endif(PROPAGATE)
endfunction(_ac_add_include_path TARGET PATH)

# add library path to a target for its own use
# PROPAGATE indicates if this path should be used by dependents
function(_ac_add_library_path TARGET PATH PROPAGATE)
    _ac_prepend_target_flag(${TARGET} LINK_FLAGS "-L\"${PATH}\"")
    if(PROPAGATE)
        prepend_global(${TARGET}_LIBRARY_DIRS "${PATH}")
    endif(PROPAGATE)
endfunction(_ac_add_library_path TARGET PATH)

# add build-directory-global include and library paths to target
function(_ac_add_global_paths TARGET)
    foreach(INCLUDE_DIR ${ACMAKE_GLOBAL_INCLUDE_DIRS})
        _ac_add_include_path(${TARGET} "${INCLUDE_DIR}" NO)
    endforeach(INCLUDE_DIR ${ACMAKE_GLOBAL_INCLUDE_DIRS})
    foreach(LIBRARY_DIR ${ACMAKE_GLOBAL_LIBRARY_DIRS})
        _ac_add_library_path(${TARGET} "${LIBRARY_DIR}" NO)
    endforeach(LIBRARY_DIR ${ACMAKE_GLOBAL_LIBRARY_DIRS})
endfunction(_ac_add_global_paths TARGET)

# add include and library search paths for the given
# EXTERNS and DEPENDS to the given TARGET
macro(_ac_add_dependency_paths TARGET DEPENDS EXTERNS)
    _ac_collect_dependency_paths(${TARGET} LIBRARY_DIRS INCLUDE_DIRS "${DEPENDS}" "${EXTERNS}")
    foreach(DIR ${INCLUDE_DIRS})
        _ac_add_include_path(${TARGET} "${DIR}" YES)
    endforeach(DIR)
    foreach(DIR ${LIBRARY_DIRS})
        _ac_add_library_path(${TARGET} "${DIR}" YES)
    endforeach(DIR)
endmacro(_ac_add_dependency_paths)


# collect include and library search paths for a set
# of external and internal dependencies, returning
# lists of paths in the variables indicated
# by LIBS and INCS
macro(_ac_collect_dependency_paths TARGET LIBS INCS DEPENDS EXTERNS)
    set(_EXTERNS ${EXTERNS})
    set(_LIBRARY)
    set(_INCLUDE)

    foreach(DEPEND ${DEPENDS})
        get_global(${DEPEND}_LIBRARY_DIRS ${DEPEND}_LIBRARY_DIRS)
        if(${DEPEND}_LIBRARY_DIRS)
            list(APPEND _LIBRARY ${${DEPEND}_LIBRARY_DIRS})
        endif(${DEPEND}_LIBRARY_DIRS)

        get_global(${DEPEND}_INCLUDE_DIRS ${DEPEND}_INCLUDE_DIRS)
        if(${DEPEND}_INCLUDE_DIRS)
            list(APPEND _INCLUDE ${${DEPEND}_INCLUDE_DIRS})
        endif(${DEPEND}_INCLUDE_DIRS)
    endforeach(DEPEND)

    if(_EXTERNS)
        list(REMOVE_DUPLICATES _EXTERNS)
    endif(_EXTERNS)

    foreach(EXTERN ${EXTERNS})
        if(NOT EXTERN MATCHES ".*\\.framework/?$")
            if(${EXTERN}_LIBRARY_DIRS)
                list(APPEND _LIBRARY ${${EXTERN}_LIBRARY_DIRS})
            else(${EXTERN}_LIBRARY_DIRS)
                if(${EXTERN}_LIBRARY_DIR)
                    list(APPEND _LIBRARY ${${EXTERN}_LIBRARY_DIR})
                endif(${EXTERN}_LIBRARY_DIR)
            endif(${EXTERN}_LIBRARY_DIRS)

            if(${EXTERN}_INCLUDE_DIRS)
                list(APPEND _INCLUDE ${${EXTERN}_INCLUDE_DIRS})
            else(${EXTERN}_INCLUDE_DIRS)
                if(${EXTERN}_INCLUDE_DIR)
                    list(APPEND _INCLUDE ${${EXTERN}_INCLUDE_DIR})
                endif(${EXTERN}_INCLUDE_DIR)
            endif(${EXTERN}_INCLUDE_DIRS)

        endif(NOT EXTERN MATCHES ".*\\.framework/?$")
    endforeach(EXTERN)

    if(_LIBRARY)
        list(REMOVE_DUPLICATES _LIBRARY)
    endif(_LIBRARY)
    if(_INCLUDE)
        list(REMOVE_DUPLICATES _INCLUDE)
    endif(_INCLUDE)

    set(${LIBS} ${_LIBRARY})
    set(${INCS} ${_INCLUDE})
endmacro(_ac_collect_dependency_paths)

