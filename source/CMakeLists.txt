cmake_minimum_required(VERSION 3.15)

project(AccountProgram)

add_executable(${PROJECT_NAME} main.cpp src/functions.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "/MT")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(${PROJECT_NAME} PRIVATE -static)
endif ()