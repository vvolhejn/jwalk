cmake_minimum_required(VERSION 3.13)
project(jwalk)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib/irrKlang-64bit-1.6.0/include/)
#link_directories(lib/irrKlang-64bit-1.6.0/bin/macosx-gcc/)
#add_library(irrklang SHARED IMPORTED)

add_executable(jwalk src/main.cpp src/Game.cpp src/Game.h src/Obstacle.cpp src/Obstacle.h src/Constants.h)
#target_link_directories(jwalk lib/irrKlang-64bit-1.6.0/lib/)

#target_link_libraries(jwalk irrklang)
target_link_libraries(jwalk "/Users/vaclav/mff/jwalk/lib/irrKlang-64bit-1.6.0/bin/macosx-gcc/libirrklang.dylib")
