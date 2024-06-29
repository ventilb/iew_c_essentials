cmake_minimum_required(VERSION 3.27)
project(iew_c_essentials C CXX)

set(CMAKE_C_STANDARD 11)

set(ENABLE_DEBUG ON)
set(FN_MALLOC "malloc" CACHE STRING "The 'malloc' function to use")
set(FN_ALIGNED_ALLOC "aligned_alloc" CACHE STRING "The 'aligned_alloc' function to use")
set(FN_REALLOC "realloc" CACHE STRING "The 'realloc' function to use")
set(FN_FREE "free" CACHE STRING "The 'free' function to use")
set(USE_LOG_LEVEL "TRACE" CACHE STRING "The log level used for log messages from the lib")

# No comma here damnit!!!
add_compile_definitions(
        IEW_FN_MALLOC=${FN_MALLOC}
        IEW_FN_ALIGNED_ALLOC=${FN_ALIGNED_ALLOC}
        IEW_FN_REALLOC=${FN_REALLOC}
        IEW_FN_FREE=${FN_FREE}
)

IF(ENABLE_DEBUG)
    add_compile_definitions(IEW_ENABLE_DEBUG=${ENABLE_DEBUG})
ENDIF (ENABLE_DEBUG)

IF(${USE_LOG_LEVEL} MATCHES "TRACE")
    MESSAGE(VERBOSE "Using log levels: TRACE, DEBUG, INFO, ERROR")
    add_compile_definitions(IEW_LOG_LEVEL_ERROR IEW_LOG_LEVEL_INFO IEW_LOG_LEVEL_DEBUG IEW_LOG_LEVEL_TRACE)
ELSEIF(${USE_LOG_LEVEL} MATCHES "DEBUG")
    MESSAGE(VERBOSE "Using log levels: DEBUG, INFO, ERROR")
    add_compile_definitions(IEW_LOG_LEVEL_ERROR IEW_LOG_LEVEL_INFO IEW_LOG_LEVEL_DEBUG)
ELSEIF(${USE_LOG_LEVEL} MATCHES "INFO")
    MESSAGE(VERBOSE "Using log levels: INFO, ERROR")
    add_compile_definitions(IEW_LOG_LEVEL_ERROR IEW_LOG_LEVEL_INFO)
ELSEIF(${USE_LOG_LEVEL} MATCHES "ERROR")
    MESSAGE(VERBOSE "Using log levels: ERROR")
    add_compile_definitions(IEW_LOG_LEVEL_ERROR)
ELSE(${USE_LOG_LEVEL} MATCHES "TRACE")
    MESSAGE(VERBOSE "Using log levels: None")
ENDIF(${USE_LOG_LEVEL} MATCHES "TRACE")

# --- End settings/options


# utf8.h
# Making it available in a submodule makes a lot of troubles so put it here
include(FetchContent)
FetchContent_Declare(
        utf8_h
        # Specify the commit you depend on and update it regularly.
        GIT_REPOSITORY https://github.com/sheredom/utf8.h.git
        GIT_TAG master
)

FetchContent_Declare(
        fnv_hash
        # Specify the commit you depend on and update it regularly.
        GIT_REPOSITORY https://github.com/catb0t/fnv-hash.git
        GIT_TAG master
)
FetchContent_MakeAvailable(utf8_h fnv_hash)

add_custom_target(fnv_hash COMMAND make check
        WORKING_DIRECTORY ${fnv_hash_SOURCE_DIR}
        BYPRODUCTS ${fnv_hash_SOURCE_DIR}/hash_32.o ${fnv_hash_SOURCE_DIR}/hash_32a.o ${fnv_hash_SOURCE_DIR}/hash_64.o ${fnv_hash_SOURCE_DIR}/hash_64a.o
)

SET(fnv_hash.o ${fnv_hash_SOURCE_DIR}/hash_32.o ${fnv_hash_SOURCE_DIR}/hash_32a.o ${fnv_hash_SOURCE_DIR}/hash_64.o ${fnv_hash_SOURCE_DIR}/hash_64a.o)

add_library(libiewcessentials-static STATIC
        vec_macros.h
        col_error.h
        vec_uint64.c
        vec_uint64.h
        vec_char.c
        vec_char.h
        vec_string.c
        vec_string.h
        icelogging.c
        icelogging.h
        icestring.c
        icestring.h
        icehash.c
        icehash.h
        ${utf8_h_SOURCE_DIR}/utf8.h
        ${fnv_hash_SOURCE_DIR}/fnv.h
        icemalloc.h
        icemalloc.c
        icealignedarray.c
        icealignedarray.h
        vec_uintptr.c
        vec_uintptr.h
        vec_float.c
        vec_float.h
        buf_macros.h
        ice_hash_table_macros.h
        vec_int.c
        vec_int.h
        ice_bits.h
)

add_dependencies(libiewcessentials-static fnv_hash)

target_link_libraries(libiewcessentials-static
        PRIVATE ${utf8_h_SOURCE_DIR}/utf8.h
        PRIVATE ${fnv_hash.o}
)

target_include_directories(libiewcessentials-static
        PUBLIC ${CMAKE_SOURCE_DIR}
        PUBLIC ${utf8_h_SOURCE_DIR}
        PUBLIC ${fnv_hash_SOURCE_DIR}
)

add_subdirectory(Google_tests)
