cmake_minimum_required(VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(STRINGS "binName.txt" PROJECT_NAME)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

message(${CMAKE_CURRENT_SOURCE_DIR})

set(PATH_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(PATH_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

message(${PATH_SRC}/main.cpp)

set(SOURCES 
  ${PATH_SRC}/main.cpp
  ${PATH_SRC}/notifications.cpp
  ${PATH_SRC}/mem.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${PATH_INCLUDE}
)
