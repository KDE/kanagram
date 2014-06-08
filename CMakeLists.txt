project(kanagram)

option(BUILD_KANAGRAM_QML "Build Kanagram QML" ON)

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

install(FILES kanagram.appdata.xml DESTINATION ${SHARE_INSTALL_PREFIX}/appdata/)
