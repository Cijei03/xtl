cmake_minimum_required(VERSION 3.24)

include("../revision.cmake")

project(xtlp VERSION ${REVISION} LANGUAGES Java)

find_package(Java 21 REQUIRED)
include(UseJava)

set(${PROJECT_NAME}_SRC 
	"xtlp/xtlpNode.java"
	"xtlp/xtlpRoot.java"
)

add_jar(lib${PROJECT_NAME}-${PROJECT_VERSION}
	SOURCES ${${PROJECT_NAME}_SRC}
)

if (${GENERATE_TEST})
	add_jar(test 
		SOURCES "Test.java"
		ENTRY_POINT Test
		INCLUDE_JARS lib${PROJECT_NAME}-${PROJECT_VERSION}.jar
	)	
endif()