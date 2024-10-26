# make doc_doxygen optional if someone does not have / like doxygen

# TODO: create CMake build option for the target.

# TODO: Add a custom target for building the documentation.

# create make target

find_package(Doxygen) #sets the variable DOXYGEN_FOUND

option(DOXY "enable doxygen" true)  #This only applies if DOXY has not been set (via command line or in the cache)

if (DOXY AND DOXYGEN_FOUND) 
add_custom_target(doc_doxygen "" EXCLUDE_FROM_ALL 
                    COMMAND doxygen
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    COMMENT 
                    "generating doxygen" 
                    VERBATIM)
endif()





