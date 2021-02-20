# CMakeList.txt : CMake project for vulkan_guide, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Setup BulletML paths


file(GLOB_RECURSE BULLETML_SOURCE_FILES 
"${CMAKE_CURRENT_SOURCE_DIR}/*.c" 
"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"  
"${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

list(FILTER BULLETML_SOURCE_FILES EXCLUDE REGEX "test")

add_library(bulletML STATIC ${BULLETML_SOURCE_FILES})

target_include_directories(bulletML PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

