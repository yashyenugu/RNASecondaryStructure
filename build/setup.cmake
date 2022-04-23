include (compilers)
include (buildtype)

set(BUILD_SHARED_LIBS OFF)
set(SHARED_LIBS_INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

if (CC_IS_GCC)
    message(STATUS "Using GCC ${CMAKE_CXX_COMPILER_VERSION}")

    set (CMAKE_CXX_FLAGS_DEBUG      "-g")
    set (CMAKE_CXX_FLAGS_RELEASE    "-O2")

elseif (CC_IS_MSVC)
    message(STATUS "Using MSVC ${CMAKE_CXX_COMPILER_VERSION}")

    set (CMAKE_CXX_FLAGS                "/MP /EHsc /utf-8")
    set (CMAKE_CXX_FLAGS_DEBUG          "/MP /Zi /Ob0 /Od /RTC1")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "/MT /Zi /O2 /Ob1")
    set (CMAKE_CXX_FLAGS_RELEASE        "/MT /O22 /Ob2")
    set (CMAKE_EXE_LINKER_FLAGS         "/DYNAMICBASE:NO")

    add_definitions(-DWIN32)
    add_definitions(-D_CONSOLE)
    add_definitions(-D_UNICODE)
    add_definitions(-DUNICODE)
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DNOMINMAX)

elseif (CC_IS_MINGW)
    message(STATUS "Using Compiler MINGW ${CMAKE_CXX_COMPILER_VERSION}")

    set(CMAKE_CXX_FLAGS_DEBUG   "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -mno-ms-bitfields")

    add_definitions(-D_UNICODE)
    add_definitions(-DUNICODE)

elseif (CC_IS_CLANG)
    message(STATUS "Using Compiler CLANG ${CMAKE_CXX_COMPILER_VERSION}")

    set(CMAKE_CXX_FLAGS_DEBUG   "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")

else ()
    message(FATAL_ERROR "Unsupported Compiler CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

endif()

include (warnings)

string (TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

if (CMAKE_BUILD_TYPE MATCHES "RELEASE")
    add_definitions(-DNDEBUG)

endif ()