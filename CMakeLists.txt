# Setup project
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Genetic_Image_Draw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set build type variable in preprocessor
if(CMAKE_BUILD_TYPE)
    string(TOUPPER "${CMAKE_BUILD_TYPE}" _upper_build_type)
    add_compile_definitions(BUILD_${_upper_build_type}=1)
endif()

# allow OpenMP
OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
  FIND_PACKAGE(OpenMP)
  IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    add_compile_definitions(cimg_use_openmp=1)
  ENDIF()
ENDIF()

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
