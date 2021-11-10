# CMake build system design considerations:
# - Include directories:
#   + Do not define include directories globally using the include_directories
#     command but rather at the target level using the
#     target_include_directories command. That way, it is easier to guarantee
#     that targets are built using the proper list of include directories.
#   + Use the PUBLIC and PRIVATE keywords to specify the scope of include
#     directories. That way, a target linking to a library (using the
#     target_link_librairies command) inherits from the library PUBLIC include
#     directories and not from the PRIVATE ones.

cmake_minimum_required(VERSION 3.2)

# use git version as library version
find_package(Git QUIET)
if (Git_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    execute_process(
            COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE git_version
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    message(Git\ hash:\ ${GIT_HASH})
else ()
    set(git_version 0)
endif ()

project(neoworld LANGUAGES C CXX VERSION 0.2.${git_version})

SET(CMAKE_CXX_FLAGS "-O -msse3 -DFP_TYPE=float -Wno-unused-result")

configure_file(src/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/build/config.h)

add_executable(neoworld
        src/world/cheaptrick.h
        src/world/common.h
        src/world/constantnumbers.h
        src/world/d4c.h
        src/world/dio.h
        src/world/fft.h
        src/world/macrodefinitions.h
        src/world/matlabfunctions.h
        src/world/stonemask.h
        src/world/synthesis.h
        src/world.h
        src/matlabmyfunctions.h
        src/audio_io.h
        src/audio_io.cpp
        src/cheaptrick.cpp
        src/common.cpp
        src/d4c.cpp
        src/dio.cpp
        src/fft.cpp
        src/star.cpp
        src/matlabfunctions.cpp
        src/matlabmyfunctions.cpp
        src/stonemask.cpp
        src/synthesis.cpp
        src/getWorldValues77.cpp
        src/getWorldValues77.h
        src/windows.h
        src/main.cpp
)