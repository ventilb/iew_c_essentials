project(Google_tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(iew_c_essentials_tests
        vec_uint64_t_test.cc
        vec_char_test.cc
        string_test.cc
        hash_test.cpp
)
target_link_libraries(iew_c_essentials_tests gtest_main iew_c_essentials)
add_test(NAME iew_c_essentials_tests COMMAND iew_c_essentials_tests)