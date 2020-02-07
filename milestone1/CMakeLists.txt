cmake_minimum_required(VERSION 3.0)
project(cj-- VERSION 1.0 LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 
	../out/
)


#[[
        Headers
]]
set(HEADERS
	include/token.h
	include/scanner.h
)


#[[
        Sources
]]
set(SOURCES
	src/main.cpp
	src/token.cpp
	src/scanner.cpp
)

#[[
        Executable
]]
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})

target_include_directories(${PROJECT_NAME}
	PRIVATE include
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)