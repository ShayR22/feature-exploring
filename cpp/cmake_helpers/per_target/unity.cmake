function(unity_group GROUP_NAME)
    if(NOT USE_UNITY_BUILD)
        return()
    endif()

    cmake_parse_arguments(UNITY "" "TARGET" "" ${ARGN})
    if(NOT UNITY_TARGET)
        set(UNITY_TARGET ${PROJECT_NAME})
    endif()

    set_source_files_properties(${UNITY_UNPARSED_ARGUMENTS} PROPERTIES UNITY_GROUP "${GROUP_NAME}")
    set_target_properties(${UNITY_TARGET} PROPERTIES
        UNITY_BUILD ON
        UNITY_BUILD_MODE GROUP
    )
endfunction()

function(unity_batch)
    if(NOT USE_UNITY_BUILD)
        return()
    endif()

    cmake_parse_arguments(UNITY "" "TARGET;BATCH_SIZE" "" ${ARGN})
    if(NOT UNITY_TARGET)
        set(UNITY_TARGET ${PROJECT_NAME})
    endif()
    if(NOT UNITY_BATCH_SIZE)
        set(UNITY_BATCH_SIZE 4)
    endif()

    set_target_properties(${UNITY_TARGET} PROPERTIES
        UNITY_BUILD ON
        UNITY_BUILD_MODE BATCH
        UNITY_BUILD_BATCH_SIZE ${UNITY_BATCH_SIZE}
    )
endfunction()

function(skip_unity)
    if(NOT USE_UNITY_BUILD)
        return()
    endif()
    set_source_files_properties(${ARGN} PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON)
endfunction()
