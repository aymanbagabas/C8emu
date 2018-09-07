cmake_minimum_required (VERSION 3.7)
set(PROJECT1 C8emu)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${${PROJECT_NAME}_SOURCE_DIR}/cmake")

# Look up SDL2 and add the include directory to our include path
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

# C8emu
include_directories(src/gui)
add_executable(${PROJECT1} main.cpp src/C8.cpp src/C8.h src/Emulator.cpp src/Emulator.h src/gui/Input.cpp src/gui/Input.h src/gui/Display.cpp src/gui/Display.h)
target_link_libraries(${PROJECT1} ${SDL2_LIBRARY})

