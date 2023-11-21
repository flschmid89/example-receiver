cmake_minimum_required(VERSION 3.14)

set(PROJECT "Example_Opticus")

project("${PROJECT}_Project" VERSION 1.0.0 LANGUAGES CXX)

# Global CMake variables are set here
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

# Options





#OPENCV
find_package(OpenCV REQUIRED)
#CPPZMQ ZEROMQ WRAPPER
find_package(cppzmq REQUIRED)
find_package(Threads)








# Loguru
set(LOGURU_SOURCES "external/loguru/loguru.cpp")
set(LOGURU_HEADERS "external/loguru/loguru.hpp")
set(LOGURU_INCLUDES "external/loguru/")
add_library(loguru ${LOGURU_SOURCES} ${LOGURU_HEADERS})




set(JSON_Install OFF CACHE INTERNAL "")

add_subdirectory(external/json)

# Project/Library Names
set (APP ${PROJECT}_Executable )


set (APP_SOURCES 
            "main.cpp"
            "src/connection_lib.cpp")

set (APP_INCLUDES "include/")

set (APP_HEADERS 
            "include/connection_lib.hpp"
            "include/dispatcher_lib.hpp")

add_executable(${APP} "main.cpp" ${APP_SOURCES} ${APP_HEADERS} )

target_include_directories(${APP} PUBLIC 
            ${APP_INCLUDES} 
            ${OpenCV_INCLUDE_DIRS}
            ${NLOHMANN_JSON_INCLUDE_DIRS} 
            ${LIBRARY_INCLUDES} 
            ${HIREDIS_HEADER} 
            ${LOGURU_INCLUDES})


target_link_libraries(${APP} PUBLIC 
    loguru     
    cppzmq 
    pthread 
    stdc++fs
    nlohmann_json::nlohmann_json 
    ${OpenCV_LIBS} 


)
target_compile_features(${APP} PUBLIC cxx_std_20)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

