cmake_minimum_required(VERSION 3.10)
project(OptionPricing CUDA CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 14)

find_package(CUDA REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCES
    src/data_processing.cpp
    src/option_pricing.cpp
    src/gpu_kernels.cu
    src/main.cpp
)

add_executable(option_pricing ${SOURCES})
target_link_libraries(option_pricing ${CUDA_LIBRARIES} ${Boost_LIBRARIES})