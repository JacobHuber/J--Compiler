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

#[[
target_compile_definitions(${PROJECT_NAME}
	PRIVATE -DGLFW_INCLUDE_NONE
)
]]

#[[
if (MSVC)
	target_compile_definitions(${PROJECT_NAME}
		PRIVATE -D_USE_MATH_DEFINES
		PRIVATE -DNOMINMAX
	)
endif()
]]


set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
#[[
	MACOSX_BUNDLE TRUE
	MACOSX_FRAMEWORK_IDENTIFIER org.cmake.${PROJECT_NAME}
]]

#[[
target_compile_definitions(${PROJECT_NAME}
	PRIVATE GLFW_INCLUDE_NONE
)
]]

#[[
target_link_libraries(${PROJECT_NAME}
	PRIVATE ${OPENGL_gl_LIBRARY}
	PRIVATE glfw
	PRIVATE ${GLFW_LIBRARIES}
	PRIVATE glad
	PRIVATE ${GLAD_LIBRARIES}
	PRIVATE ${CMAKE_DL_LIBS}
)
]]