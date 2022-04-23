string (TIMESTAMP SCRIPT_START_TIMESTAMP "%s" UTC)

if (NOT DEFINED CMAKE_SCRIPT_MODE_FILE)
    file (RELATIVE_PATH SCRIPT_PATH "${CMAKE_BINARY_DIR}" "${CMAKE_CURRENT_LIST_FILE}")
    message (FATAL_ERROR
        "This file is a script. You should run it with:\n"
        "  \$ cmake -P ${SCRIPT_PATH} [args...]\n"
        "Don't try to use it in other CMake files with include().\n"
        "See https://cmake.org/cmake/help/latest/manual/cmake.1.html#run-a-script"
    )
endif ()

cmake_minimum_required(VERSION 3.10)

set (i "1")
while (i LESS "${CMAKE_ARGC}")
    if ("${CMAKE_ARGV${i}}" STREQUAL "-P")
        math (EXPR i "${i} + 1")
        break ()
    endif ()
    math (EXPR i "${i} + 1")
endwhile ()

set (SCRIPT_ARGS "") 
while (i LESS "${CMAKE_ARGC}")
    list (APPEND SCRIPT_ARGS "${CMAKE_ARGV${i}}")
    math (EXPR i "${i} + 1")
endwhile ()

include("${CMAKE_CURRENT_LIST_DIR}/build/utils.cmake")

function (build_folder
    VAR_NAME
)
    if (WIN32)
        set (PLATFORM "Win")
    elseif (APPLE)
        set (PLATFORM "Mac")
    else ()
        set (PLATFORM "${CMAKE_HOST_SYSTEM_NAME}")
    endif ()

    set (GEN_SHORT "${GENERATOR}")
    string (REGEX REPLACE ".*Visual Studio ([0-9]+).*" "VS\\1" GEN_SHORT "${GEN_SHORT}")
    string (REGEX REPLACE ".*MinGW.*" "MinGW" GEN_SHORT "${GEN_SHORT}")
    string (REGEX REPLACE ".*Makefile.*" "Make" GEN_SHORT "${GEN_SHORT}")

    set (BUILD_FOLDER "${PLATFORM}-${GEN_SHORT}-${BUILD_TYPE}")
    string (REGEX REPLACE "[ /\\]" "" BUILD_FOLDER "${BUILD_FOLDER}")
    set ("${VAR_NAME}" "${BUILD_FOLDER}" PARENT_SCOPE)
endfunction ()

set(SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(ALL_BUILDS_PATH "${SOURCE_PATH}/builds")

cmake_host_system_information (RESULT CPUS QUERY NUMBER_OF_LOGICAL_CORES)

set (i "1")
list (LENGTH SCRIPT_ARGS nargs)
while (i LESS "${nargs}")
    list (GET SCRIPT_ARGS "${i}" ARG)
    if ("${ARG}" STREQUAL "clean")
        set (ARG_CLEAN "TRUE")
    elseif ("${ARG}" STREQUAL "configure")
        set (ARG_CONFIGURE "TRUE")
    elseif ("${ARG}" STREQUAL "build")
        set (ARG_CONFIGURE "TRUE")
        set (ARG_BUILD "TRUE")
    elseif ("${ARG}" STREQUAL "install")
        set (ARG_CONFIGURE "TRUE")
        set (ARG_BUILD "TRUE")
        set (ARG_INSTALL "TRUE")
    elseif ("${ARG}" STREQUAL "run")
        set (ARG_RUN "TRUE")
    else ()
        if (ARG_RUN)
            list (APPEND RUN_ARGS "${ARG}")
        else ()
            list (APPEND CONFIGURE_ARGS "${ARG}")
        endif ()
    endif ()
    math (EXPR i "${i} + 1")
endwhile ()

if (NOT (ARG_CLEAN OR ARG_CONFIGURE OR ARG_BUILD OR ARG_INSTALL OR ARG_RUN))
    set (ARG_CONFIGURE "TRUE")
    set (ARG_BUILD "TRUE")
    set (ARG_INSTALL "TRUE")
endif ()

fn__get_option(GENERATOR -G ${CONFIGURE_ARGS})
if (WIN32)
    fn__set_default (GENERATOR "Visual Studio 16 2019")
else ()
    fn__set_default (GENERATOR "Unix Makefiles")
endif ()

fn__get_option(BUILD_TYPE -DCMAKE_BUILD_TYPE ${CONFIGURE_ARGS})
fn__set_default(BUILD_TYPE "RelWithDebInfo")

if (NOT DEFINED BUILD_FOLDER)
    build_folder(BUILD_FOLDER)
endif ()
set (BUILD_PATH "${ALL_BUILDS_PATH}/${BUILD_FOLDER}")

fn__get_option (INSTALL_PATH -DCMAKE_INSTALL_PREFIX ${CONFIGURE_ARGS})
fn__set_default (INSTALL_PATH "install")

if(WIN32)
    fn__set_default(INTERSECTION_EXECUTABLE "bin/LineIntersection.exe")
elseif(APPLE)
    fn__set_default(INTERSECTION_EXECUTABLE "lineintersection.app/Contents/MacOS/lintersection")
else()
    fn__set_default(INTERSECTION_EXECUTABLE "bin/lintersection")
endif()

get_filename_component(BUILD_PATH "${BUILD_PATH}" ABSOLUTE BASE_DIR "${SOURCE_PATH}")
get_filename_component(INSTALL_PATH "${INSTALL_PATH}" ABSOLUTE BASE_DIR "${BUILD_PATH}")
get_filename_component(INTERSECTION_EXECUTABLE "${INTERSECTION_EXECUTABLE}" ABSOLUTE BASE_DIR "${INSTALL_PATH}")

message(STATUS "SOURCE_PATH:       ${SOURCE_PATH}")
message(STATUS "BUILD_PATH:        ${BUILD_PATH}")
message(STATUS "INSTALL_PATH:      ${INSTALL_PATH}")
message(STATUS "INTERSECTION_EXECUTABLE: ${INTERSECTION_EXECUTABLE}")
message(STATUS "CPUS: ${CPUS}")
message(STATUS "GENERATOR: ${GENERATOR}")
message(STATUS "BUILD_TYPE: ${BUILD_TYPE}")

list(APPEND CONFIGURE_ARGS "-G" "${GENERATOR}")
list(APPEND CONFIGURE_ARGS "-DCMAKE_INSTALL_PREFIX=${INSTALL_PATH}")
list(APPEND CONFIGURE_ARGS "-DCMAKE_BUILD_TYPE=${BUILD_TYPE}")
list(APPEND CONFIGURE_ARGS "-DBUILD_64=ON")

#### ACTUAL BUILD STEPS START HERE ####

if (ARG_CLEAN)
    message ("\n~~~~ Cleaning the the previous build ~~~~\n")
    message ("Deleting ${BUILD_PATH}")
    file (REMOVE_RECURSE "${BUILD_PATH}")
endif ()

if (ARG_CONFIGURE AND NOT EXISTS "${BUILD_PATH}/CMakeCache.txt")
    message ("\n~~~~ Configuring the project and generating the cache ~~~~\n")
    file (MAKE_DIRECTORY "${BUILD_PATH}")

    fn__command_string (ARGS_STR ${CONFIGURE_ARGS})
    message ("CONFIGURE_ARGS: ${ARGS_STR}")
    execute_process (
        COMMAND cmake -S "${SOURCE_PATH}" -B . ${CONFIGURE_ARGS}
        WORKING_DIRECTORY "${BUILD_PATH}"
        RESULT_VARIABLE EXIT_STATUS
    )
    if (NOT "${EXIT_STATUS}" EQUAL "0")
        file (REMOVE "${BUILD_PATH}/CMakeCache.txt")
        message (FATAL_ERROR "Configure step failed with status ${EXIT_STATUS}. See output above for details.")
    endif ()
endif ()

if (ARG_BUILD)
    message ("\n~~~~ Building the project ~~~~\n")
    execute_process (
        COMMAND cmake --build . --config "${BUILD_TYPE}" --parallel "${CPUS}"
        WORKING_DIRECTORY "${BUILD_PATH}"
        RESULT_VARIABLE EXIT_STATUS
    )
    if (NOT "${EXIT_STATUS}" EQUAL "0")
        message (FATAL_ERROR "Build step failed with status ${EXIT_STATUS}. See output above for details.")
    endif ()
endif ()

if (ARG_INSTALL)
    message("\n~~~~ Installing the built application ~~~~\n")
    execute_process (
        COMMAND cmake --install . --config "${BUILD_TYPE}"
        WORKING_DIRECTORY "${BUILD_PATH}"
        RESULT_VARIABLE EXIT_STATUS
    )
    if (NOT "${EXIT_STATUS}" EQUAL "0")
        message (FATAL_ERROR "Install step failed with status ${EXIT_STATUS}. See output above for details.")
    endif ()
endif ()

if (ARG_RUN)
    message("\n~~~~ Running the installed application ~~~~\n")
    if (WIN32)
        set (CMD "cmd.exe" "/c")
    else ()
        set (CMD "")
    endif ()

    fn__command_string (ARGS_STR ${RUN_ARGS})
    message ("RUN_ARGS: ${ARGS_STR}")

    execute_process (
        COMMAND ${CMD} "${INTERSECTION_EXECUTABLE}" ${RUN_ARGS}
        RESULT_VARIABLE EXIT_STATUS
    )

    if (NOT "${EXIT_STATUS}" EQUAL "0")
        message (FATAL_ERROR "Run step failed with status ${EXIT_STATUS}. See output above for details.")
    endif ()
endif ()


string (TIMESTAMP SCRIPT_END_TIMESTAMP "%s" UTC)
math (EXPR SCRIPT_ELAPSED_TIME "${SCRIPT_END_TIMESTAMP} - ${SCRIPT_START_TIMESTAMP}")
math (EXPR SCRIPT_ELAPSED_MINS "${SCRIPT_ELAPSED_TIME} / 60")
math (EXPR SCRIPT_ELAPSED_SECS "${SCRIPT_ELAPSED_TIME} % 60")
list (GET SCRIPT_ARGS "0" SCRIPT_NAME)
message ("\n${SCRIPT_NAME}: Complete after ${SCRIPT_ELAPSED_MINS} minutes and ${SCRIPT_ELAPSED_SECS} seconds")