# CMakeList.txt : CMake project for vulkan_guide, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("rRootage")



# setup SDL library
set(SDL_PATH "K:/Programming/SDL-1.2.15" CACHE  PATH "path to SDL installation")
set(SDLMIXER_PATH "K:/Programming/SDL_mixer-1.2.12" CACHE  PATH "path to SDL Mixer installation")

add_library(SDLlibs INTERFACE)

target_link_directories(SDLlibs INTERFACE "${SDL_PATH}/lib/x64" "${SDLMIXER_PATH}/lib/x64")
target_link_libraries(SDLlibs INTERFACE SDL SDLmain SDL_mixer)
target_include_directories(SDLlibs INTERFACE "${SDL_PATH}/include" "${SDLMIXER_PATH}/include")



add_subdirectory(src)
