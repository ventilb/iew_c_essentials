cmake_minimum_required(VERSION 3.27)
project(iew_c_essentials C CXX)

set(CMAKE_C_STANDARD 11)

set(ENABLE_DEBUG ON)
set(FN_MALLOC "malloc" CACHE STRING "The 'malloc' function to use")
set(FN_ALIGNED_ALLOC "aligned_alloc" CACHE STRING "The 'aligned_alloc' function to use")
set(FN_REALLOC "realloc" CACHE STRING "The 'realloc' function to use")
set(FN_FREE "free" CACHE STRING "The 'free' function to use")

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
)

SET(fnv_hash.o ${fnv_hash_SOURCE_DIR}/hash_32.o ${fnv_hash_SOURCE_DIR}/hash_32a.o ${fnv_hash_SOURCE_DIR}/hash_64.o ${fnv_hash_SOURCE_DIR}/hash_64a.o)

add_library(libiewcessentials-static STATIC
        utils.c
        vec_uint64_t.c
        vec_char.c
        icestring.c
        hash.h
)

add_dependencies(libiewcessentials-static fnv_hash)

target_link_libraries(libiewcessentials-static
        PUBLIC ${utf8_h_SOURCE_DIR}/utf8.h
        PUBLIC ${fnv_hash.o}
)

target_include_directories(libiewcessentials-static
        PUBLIC ${utf8_h_SOURCE_DIR}
        PUBLIC ${fnv_hash_SOURCE_DIR}
)

add_subdirectory(Google_tests)

