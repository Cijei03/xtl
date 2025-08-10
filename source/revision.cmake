set(REVISION 100)
set(CMAKE_BUILD_TYPE Release)

option(GENERATE_TEST "Build also test executable.")
option(SANITIZER_ENABLED "If set ON enables sanitizer option when build. Applies only when compiling for C or C++ target.")

message(STATUS "General settings of build:")
message(STATUS "XTL revision: ${REVISION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Checking for options...")
message(STATUS "GENERATE_TEST=${GENERATE_TEST}")
message(STATUS "SANITIZER_ENABLED=${SANITIZER_ENABLED}")