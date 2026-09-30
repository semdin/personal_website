cmake_minimum_required(VERSION 3.25)
project(personal_website VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
        -Wno-c++26-extensions
    )
endif()

# Fetch twf framework from GitHub (Just like npm packages in modern web frameworks!)
include(FetchContent)
FetchContent_Declare(
    twf
    GIT_REPOSITORY https://github.com/semdin/twf.git
    GIT_TAG        v0.1.1
)
FetchContent_MakeAvailable(twf)

# Backend Server Executable
add_executable(my_website src/server/main.cpp)
target_link_libraries(my_website PRIVATE twf::twf)
target_include_directories(my_website PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# On Windows, link winsock
if(WIN32)
    target_link_libraries(my_website PRIVATE ws2_32 mswsock)
endif()
