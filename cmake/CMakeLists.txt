
include(FetchContent)
# collect all cpp files in tests
file(GLOB_RECURSE TEST_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Calculator/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Objects/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/Objects/ParticleGeneratorTest.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Particle.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Cuboid.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ParticleContainer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/DirectSumContainer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ParticleGenerator.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Calculator/LennardJonesCalculator.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Calculator/gravityCalculator.cpp"
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

if(LVL STREQUAL ERROR)
        set(LOG_LEVEL SPDLOG_LEVEL_ERROR)
elseif(LVL STREQUAL WARN)
        set (LOG_LEVEL SPDLOG_LEVEL_WARN)
elseif(LVL STREQUAL OFF)
        set (LOG_LEVEL SPDLOG_LEVEL_OFF)
elseif(LVL STREQUAL DEBUG)
        set (LOG_LEVEL SPDLOG_LEVEL_DEBUG)
elseif(LVL STREQUAL TRACE)
        set (LOG_LEVEL SPDLOG_LEVEL_TRACE)
elseif(LVL STREQUAL ALL)
        set (LOG_LEVEL SPDLOG_LEVEL_TRACE)
else()
        set (LOG_LEVEL SPDLOG_LEVEL_INFO)
endif()

target_compile_options(Test
        PRIVATE
        $<$<CXX_COMPILER_ID:GNU>: -Wall -g -D SPDLOG_ACTIVE_LEVEL=${LOG_LEVEL}>)

include(GoogleTest)
gtest_discover_tests(Test)
include(CTest)