project(kanagram)

option(BUILD_KANAGRAM_DESKTOP       "Build Kanagram for KDE Desktop" ON)
option(BUILD_KANAGRAM_HARMATTAN     "Build Kanagram for Harmattan" OFF)
option(BUILD_KANAGRAM_PLASMA_ACTIVE "Build Kanagram for Plasma Active" OFF)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -Wno-switch -O2 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wno-switch -O2")
    set(CMAKE_CXX_FLAGS_DEBUG  "-Wall -Wno-switch -g3 -ggdb -O0" CACHE STRING "Debug options." FORCE)
endif(NOT MSVC)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

find_package(LibKdeEdu REQUIRED)
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

add_subdirectory( doc )
add_subdirectory( images ) 
add_subdirectory( src ) 
add_subdirectory( icons ) 
add_subdirectory( sounds ) 
