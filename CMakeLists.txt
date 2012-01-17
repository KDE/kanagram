project(kanagram)

option(BUILD_KANAGRAM_HARMATTAN "Build Kanagram for Harmattan" OFF)
option(BUILD_KANAGRAM_DESKTOP "Build Kanagram for KDE Desktop" ON)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

set(CMAKE_MODULE_PATH ${kanagram_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(LibKdeEdu REQUIRED)
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

add_subdirectory( doc )
add_subdirectory( images ) 
add_subdirectory( src ) 
add_subdirectory( icons ) 
add_subdirectory( sounds ) 


