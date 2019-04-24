cmake_minimum_required(VERSION 3.6)
project(jwalk)

set(CMAKE_CXX_STANDARD 14)

set(IRRKLANG_DIR ${CMAKE_SOURCE_DIR}/lib/irrKlang-64bit-1.6.0)
include_directories(${IRRKLANG_DIR}/include/)

add_executable(jwalk src/main.cpp src/Game.cpp src/Game.h src/Obstacle.cpp src/Obstacle.h src/Constants.h src/IO.cpp src/IO.h)

if ((${CMAKE_SYSTEM_NAME} STREQUAL "Linux") OR (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin"))
    find_package(Curses REQUIRED)
    target_link_libraries(jwalk ${CURSES_LIBRARIES})
    include_directories(${CURSES_INCLUDE_DIR})
endif()


if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    # -pthread; see https://stackoverflow.com/questions/1620918/cmake-and-libpthread
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(jwalk Threads::Threads)

    target_link_libraries(jwalk "${IRRKLANG_DIR}/bin/linux-gcc-64/libIrrKlang.so")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    target_link_libraries(jwalk "${IRRKLANG_DIR}/bin/macosx-gcc/libirrklang.dylib")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(jwalk "${IRRKLANG_DIR}/bin/winx64-visualStudio/irrKlang.dll")
else()
    message("Unknown system!")
endif()
