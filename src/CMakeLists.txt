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

# copy exe to the root folder, both windows and linux
add_custom_command(TARGET rR POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rR> "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

# only copy the DLLs on windows
if( WIN32 )
    # copy dlls to the root folder
    add_custom_command(TARGET rR POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${SDLMAIN_DLL_PATH}/SDL.dll" "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )
    add_custom_command(TARGET rR POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${SDLMIXER_DLL_PATH}/SDL_mixer.dll" "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )

    # copy dlls to the bin folder
    add_custom_command(TARGET rR POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${SDLMAIN_DLL_PATH}/SDL.dll" $<TARGET_FILE_DIR:rR>
    )
    add_custom_command(TARGET rR POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${SDLMIXER_DLL_PATH}/SDL_mixer.dll" $<TARGET_FILE_DIR:rR>
    )
endif()


if(NOT MSVC)
        set_target_properties(rR PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-deprecated-declarations")
endif()

