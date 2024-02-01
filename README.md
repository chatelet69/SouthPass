# SouthPass

Small student project in C/C++. It's a graphical password manager for Windows and Linux.

## Installation
Pkg-config can be useful to facilitate cmake
<br>You'll need a cookie folder in the user folder, and the roo file in the main folder if you need it.

(Below, a base for Cmake, it must surely be adapted or completed depending on your configurations)<br>
After configuring cmake correctly, you can build with cmake and make like this:<br>
`cmake -B ./build -G "MinGW Makefiles" && mingw32-make.exe -C ./build/`

## Authors

- [@L4p1s](https://www.github.com/PommePain)
- [@SpyKo_](https://github.com/Spykoninho)

## Techno / Language
<p>
    <img src="https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white">
    <img src="https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white">
    <img src="https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white">
</p>

## Server & Database

<img src="https://img.shields.io/badge/Debian-D70A53?style=for-the-badge&logo=debian&logoColor=white">
<p><img src="https://img.shields.io/badge/MariaDB-003545?style=for-the-badge&logo=mariadb&logoColor=white">
<img src="https://img.shields.io/badge/mysql-%2300f.svg?style=for-the-badge&logo=mysql&logoColor=white"></p>

## License
CC-by-nc

### Cmake Base
(There are maybe some unnecessary lines, it was our first cmake)

```cmake
cmake_minimum_required(VERSION 3.22)
set(NAME_PROJECT SouthPass)
project(${NAME_PROJECT} C CXX)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/g++.exe")
    set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_C_COMPILER "/usr/bin/g++")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()

file(GLOB HEADERS_FILES "./includes/*.h")
file(GLOB HEADER_CPP_FILES "./includes/*.hpp")

file(GLOB C_SOURCES "src/*.c")
file(GLOB CPP_SOURCES "src/*.cpp")
file(GLOB SUB_CPP "src/*/*.cpp")
file(GLOB SUB_C "src/*/*.c")

set(QML_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/components)
set(QML_DEST ${CMAKE_CURRENT_BINARY_DIR}/components)
set(ASSETS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/assets)
set(ASSETS_DEST ${CMAKE_CURRENT_BINARY_DIR}/assets)
set(CSS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/frontend/css)

file(COPY ${ASSETS_SOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${QML_SOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CSS_SOURCES}/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/style)

add_executable(${NAME_PROJECT} ${C_SOURCES} ${CPP_SOURCES} ${SUB_CPP} ${SUB_C} ${HEADER_CPP_FILES} ${HEADERS_FILES} icon/app.o includes/icon_data.h)
target_include_directories(${NAME_PROJECT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ./includes)
set_target_properties(${NAME_PROJECT} PROPERTIES WIN32_EXECUTABLE TRUE)    # to not open a terminal when exec

file(GLOB LIBS_DLL ${CMAKE_CURRENT_SOURCE_DIR}/libs/*.dll)

find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(cJSON REQUIRED)
pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)
target_link_libraries(${NAME_PROJECT} PRIVATE OpenSSL::Crypto OpenSSL::SSL)
target_include_directories(${NAME_PROJECT} PRIVATE ${CJSON_INCLUDE_DIRS})
target_link_libraries(${NAME_PROJECT} PRIVATE ${CJSON_LIBRARIES})

#add_definitions(-DQT_NO_KEYWORDS)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Qml QuickWidgets Quick QuickControls2)
include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS})
target_link_libraries(${NAME_PROJECT} PRIVATE ${LIBS_DLL} Qt6::Core Qt6::Widgets Qt6::Quick Qt6::Qml Qt6::QuickWidgets Qt6::QuickControls2)

foreach(FLAG ${LIBMYSQLCLIENT_CFLAGS_OTHER})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
endforeach()

target_include_directories(${NAME_PROJECT} PRIVATE ${LIBMYSQLCLIENT_INCLUDE_DIRS})
target_link_libraries(${NAME_PROJECT} PRIVATE ${LIBMYSQLCLIENT_LIBRARIES})

target_include_directories(${NAME_PROJECT} PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(${NAME_PROJECT} PRIVATE ${CURL_LIBRARIES})
```
