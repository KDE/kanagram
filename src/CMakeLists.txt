cmake_minimum_required(VERSION 2.8)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

####### Kanagram Engine ########
add_subdirectory(engine)

if(BUILD_KANAGRAM_QML)
    find_package(KDE4)
    if(KDE4_FOUND)
        include_directories(${KDE4_INCLUDES})
        add_subdirectory(qml)
    else(KDE4_FOUND)
        message(STATUS "KDE libraries are not found - not building the KDE Desktop frontend")
    endif(KDE4_FOUND)
endif()

