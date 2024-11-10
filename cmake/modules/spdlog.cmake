
include(FetchContent)

# fetch github repository
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.x  # This fetches the latest version on the main branch
        PREFIX spdlog
)

FetchContent_MakeAvailable(spdlog)

target_link_libraries(MolSim 
        PRIVATE
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

target_compile_definitions(MolSim PRIVATE SPDLOG_ACTIVE_LEVEL=${LOG_LEVEL})