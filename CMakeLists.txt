cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_COMPILER "C:/mingw64/bin/gcc.exe")
project(BozoC C)

set(MSYS2_PATH "C:/msys64")
set(CMAKE_C_STANDARD 17)

#execute_process(
#    COMMAND ${CMAKE_C_COMPILER} -E -x c - -v
#    OUTPUT_VARIABLE CompilerVerboseOutput
#    ERROR_VARIABLE CompilerErrorOutput) 

set(INCLUDE_DIRECTORIES "C:/mingw64/include")
set(LINK_DIRECTORIES "C:/mingw64/lib")

set(MSYS_INCLUDE_DIRECTORIES ${MSYS2_PATH}/mingw64/include)
set(MSYS_LINK_DIRECTORIES ${MSYS2_PATH}/mingw64/lib)
set(GTK_INCLUDE_DIR ${MSYS2_PATH}/mingw64/include/gtk-4.0)
set(GLIB_CONFIG_DIR ${MSYS2_PATH}/mingw64/lib/glib-2.0/include)

set(HEADER_FILES "./includes/db.h")

include_directories(INCLUDE_DIRECTORIES)
include_directories(${MSYS_INCLUDE_DIRECTORIES})
include_directories(${GTK_INCLUDE_DIR})
link_directories(LINK_DIRECTORIES)
link_directories(${MSYS_LINK_DIRECTORIES})

file(GLOB SOURCES "src/*.c")
file(GLOB SUBSOURCES "src/*/*.c")
file(GLOB LIB_INCLUDE_DIRS ${MSYS2_PATH}/mingw64/include/*)
file(GLOB LIB_CONFIG_INCLUDE_DIRS ${MSYS2_PATH}/mingw64/lib/*/include)

add_executable(app ${SOURCES} ${SUBSOURCES} icon/app.o)
#set_target_properties(app PROPERTIES WIN32_EXECUTABLE TRUE)

#set(ICON_FILE ${CMAKE_SOURCE_DIR}/src/gui/icon.ico)
#configure_file(${ICON_FILE} ${CMAKE_CURRENT_BINARY_DIR}/icon.ico COPYONLY)
#target_sources(app PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/icon.ico)

foreach(INCLUDE_DIR ${LIB_INCLUDE_DIRS})
    if (IS_DIRECTORY ${INCLUDE_DIR}) 
        target_include_directories(app PRIVATE ${INCLUDE_DIR})
    endif()
endforeach()

foreach(INCLUDE_DIR ${LIB_CONFIG_INCLUDE_DIRS})
    if (IS_DIRECTORY ${INCLUDE_DIR}) 
        target_include_directories(app PRIVATE ${INCLUDE_DIR})
    endif()
endforeach()

file(GLOB LIBS_DLL ${CMAKE_CURRENT_SOURCE_DIR}/libs/*.dll)
#file(GLOB TEST_DLL "C:/msys64/mingw64/bin/*.dll")

target_link_libraries(app ${LIBS_DLL})