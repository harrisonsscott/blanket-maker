cmake_minimum_required(VERSION 3.10)

project(blanketMaker)

set(OpenCV_DIR $ENV{OPENCV})
find_package(OpenCV REQUIRED)
add_executable(${PROJECT_NAME} ./main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})

set(DWITH_FFMPEG ON)
set(DFFMPEG ON)
set(DWITH_OPENCL ON)
set(DHAVE_FFMPEG ON)

include_directories(./json/include)
