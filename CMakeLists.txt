cmake_minimum_required(VERSION 3.6)
project(jwalk)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib/irrklang/include/)

add_executable(jwalk src/main.cpp src/Game.cpp src/Game.h src/Obstacle.cpp src/Obstacle.h src/Constants.h)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    # -pthread; see https://stackoverflow.com/questions/1620918/cmake-and-libpthread
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(jwalk Threads::Threads)

    target_link_libraries(jwalk "${CMAKE_SOURCE_DIR}/lib/irrklang/bin/linux-gcc-64/libIrrKlang.so")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    target_link_libraries(jwalk "${CMAKE_SOURCE_DIR}/lib/irrklang/bin/macosx-gcc/libirrklang.dylib")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(jwalk "${CMAKE_SOURCE_DIR}/lib/irrklang/bin/winx64-visualStudio/irrKlang.dll")
else()
    message("Unknown system!")
endif()
