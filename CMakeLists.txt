# Setup project
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Genetic_Image_Draw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(X11 REQUIRED)

# add include dir
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${X11_INCLUDE_DIR})

# add libraries
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${X11_LIBRARIES})

# add sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

# add executable
add_executable(drawer drawer.cpp ${sources} ${headers})

# add libraries
set(CIMG_LIBS X11 pthread)
target_link_libraries(drawer ${CIMG_LIBS})
