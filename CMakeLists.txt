cmake_minimum_required (VERSION 3.7)
set(PROJECT1 C8emu)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_executable(${PROJECT1} main.cpp src/C8.cpp src/C8.h src/Emulator.cpp src/Emulator.h src/gui/Input.cpp src/gui/Input.h src/gui/Display.cpp src/gui/Display.h)

# Look up SDL2 and add the include directory to our include path
include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

target_link_libraries(${PROJECT1} ${SDL2_LIBRARIES})
include_directories(${SDL2_INCLUDE_DIRS} src src/gui)
