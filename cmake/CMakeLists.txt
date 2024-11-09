
include(FetchContent)
# collect all cpp files in tests
file(GLOB_RECURSE TEST_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Particle.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ParticleContainer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
# fetch Googletest from the github repository
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        main  # This fetches the latest version on the main branch
)

# Download and add GoogleTest to the build
FetchContent_MakeAvailable(googletest)

# create testing executable
add_executable(Test ${TEST_SRC})

target_include_directories(Test
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Test PRIVATE
        gtest
        gtest_main
        spdlog
)

include(GoogleTest)
gtest_discover_tests(Test)
include(CTest)