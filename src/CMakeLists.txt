# CMakeList.txt : CMake project for vulkan_guide, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_subdirectory(bulletml)


file(GLOB SOURCE_FILES 
"${CMAKE_CURRENT_SOURCE_DIR}/*.c" 
"${CMAKE_CURRENT_SOURCE_DIR}/*.cc"  
"${CMAKE_CURRENT_SOURCE_DIR}/*.h" )


add_executable(
    rR    
	 ${SOURCE_FILES}
    )
	

	
if(NOT MSVC)
target_link_libraries(rR SDLlibs bulletML GL GLU)
else()
target_link_libraries(rR SDLlibs bulletML opengl32 glu32)
endif()
set_property(TARGET rR PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")


if(NOT MSVC)
        
        set_target_properties(rR PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-deprecated-declarations")
endif()

