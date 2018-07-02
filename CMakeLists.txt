cmake_minimum_required(VERSION 2.8)

project(Athena)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -g")

# SDL2, SDL2/SDL2_Image, SDL2/SDL2_ttf
find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2IMAGE_INCLUDE_DIR}
        ${SDL2TTF_INCLUDE_DIR})

# Athena
include_directories(src/)
include_directories(include/)
file(GLOB_RECURSE SOURCES src/*.cpp src/*.c include/*.cpp include/*.c)
file(GLOB_RECURSE HEADERS src/*.hpp src/*.h src/*.hpp include/*.hpp include/*.h)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
#add_executable(${PROJECT_NAME} "src/TrGame.cpp" ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARY}
        ${SDL2IMAGE_LIBRARY}
        ${SDL2TTF_LIBRARY})

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
