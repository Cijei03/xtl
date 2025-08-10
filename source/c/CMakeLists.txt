cmake_minimum_required(VERSION 3.24)

include("../revision.cmake")

if (${GENERATE_TEST})
	message(STATUS "GENERATE_TEST enabled. Currently this target does not support it.")
endif()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED true)

project(xtlpc VERSION ${REVISION} LANGUAGES C)

set(NAME ${PROJECT_NAME}-${CMAKE_PROJECT_VERSION})
 
add_library(${NAME})

target_sources(${NAME} PRIVATE
	"sources/xtlp-general.c"
	"sources/xtlp-r100.c"
)

if ((${SANITIZER_ENABLED} OR ${CMAKE_BUILD_TYPE} STREQUAL "Debug") AND ${CMAKE_HOST_LINUX})
	add_compile_options(-fsanitize=address -g)
	add_link_options(-fsanitize=address -static-libasan)
endif()

set_target_properties(${NAME} PROPERTIES PUBLIC_HEADER "include/xtlp-100.h")

if(${CMAKE_HOST_LINUX})
	file(CREATE_LINK ${CMAKE_STATIC_LIBRARY_PREFIX}${NAME}${CMAKE_STATIC_LIBRARY_SUFFIX} ${CMAKE_STATIC_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX} SYMBOLIC)
	install(TARGETS ${NAME} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
	install(FILES ${CMAKE_STATIC_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX} ${CMAKE_STATIC_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX} TYPE LIB)
endif()

