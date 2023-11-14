cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_COMPILER "C:/mingw64/bin/gcc.exe")
project(BozoC C)

set(CMAKE_C_STANDARD 17)

#execute_process(
#    COMMAND ${CMAKE_C_COMPILER} -E -x c - -v
#    OUTPUT_VARIABLE CompilerVerboseOutput
#    ERROR_VARIABLE CompilerErrorOutput) 

set(INCLUDE_DIRECTORIES "C:/mingw64/include")
set(LINK_DIRECTORIES "C:/mingw64/lib")

set(HEADER_FILES "./includes/db.h")

include_directories(INCLUDE_DIRECTORIES)
link_directories(LINK_DIRECTORIES)

file(GLOB SOURCES "src/*.c")
add_executable(app ${SOURCES})
target_link_libraries(app ${CMAKE_CURRENT_SOURCE_DIR}/libs/libcurl-x64.dll)
target_link_libraries(app ${CMAKE_CURRENT_SOURCE_DIR}/libs/libmysql.dll)