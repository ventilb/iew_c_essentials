project(iew_c_essentials_tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(run_iew_c_essentials_tests
        vec_uint64_test.cc
        vec_char_test.cc
        vec_string_test.cpp
        icestring_test.cc
        icehash_test.cpp
        icealignedarray_test.cpp
        buf_test.cpp
        test_data.h
)
target_link_libraries(run_iew_c_essentials_tests gtest_main libiewcessentials-static)
add_test(NAME run_iew_c_essentials_tests COMMAND run_iew_c_essentials_tests)
