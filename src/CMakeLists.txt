cmake_minimum_required(VERSION 3.14)

set(PROJECT "Opticus_Barcode")

project("${PROJECT}_Project" VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

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
set (APP ${PROJECT} )


set (APP_SOURCES 
            "main.cpp"
            "src/connection_lib.cpp"
            "src/helper_lib.cpp")

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



add_library(EdgeDecoder STATIC IMPORTED)
set_target_properties(EdgeDecoder PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/codecorp/lib/libEdgeDecoder.a)


target_link_libraries(${APP} PUBLIC 
    loguru     
    cppzmq 
    pthread 
    stdc++fs
    nlohmann_json::nlohmann_json 
    ${OpenCV_LIBS} 
    EdgeDecoder

)
target_compile_features(${APP} PUBLIC cxx_std_20)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

