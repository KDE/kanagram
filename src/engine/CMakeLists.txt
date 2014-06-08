cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

find_package(Qt4 4.7 REQUIRED)

if ("${QT_QTCORE_LIBRARY}" MATCHES lib64)
   set(_Init_LIB_SUFFIX 64)
elseif ("${QT_QTCORE_LIBRARY}" MATCHES lib32)
   set(_Init_LIB_SUFFIX 32)
endif ("${QT_QTCORE_LIBRARY}" MATCHES lib64)

set(LIB_SUFFIX
    ${_Init_LIB_SUFFIX}
    CACHE INTERNAL
    "The suffix of the system-wide library path"
)

set(BIN_INSTALL_DIR
    bin
    CACHE INTERNAL "The subdirectory relative to the install prefix where binary files will be installed."
    )

set(INCLUDE_INSTALL_DIR
    include
    CACHE INTERNAL "The subdirectory relative to the install prefix where header files will be installed."
    )

set(LIB_INSTALL_DIR
    lib${LIB_SUFFIX}
    CACHE INTERNAL "The subdirectory relative to the install prefix where libraries will be installed."
)

set(SHARE_INSTALL_DIR
    share
    CACHE INTERNAL "The subdiractory relative to the install prefix where data and other files will be installed."
)

set(KANAGRAM_ENGINE_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LIBKDEEDU_INCLUDE_DIR}
    CACHE INTERNAL "Includes required for Kanagram Engine"
)

set(KanagramEngine_SRCS
    kanagramgame.cpp
    kanagramenginehelper.cpp
)

set(KanagramEngine_HEADERS
    kanagramgame.h
    kanagram_engine_export.h
    kanagramenginehelper.h
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}

    ${LIBKDEEDU_INCLUDE_DIR}
)

kde4_add_kcfg_files(KanagramEngine_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/kanagramsettings.kcfgc)

if(APPLE)
    kde4_add_library(kanagramengine SHARED ${KanagramEngine_SRCS} ${KanagramEngine_HEADERS})
else()
    kde4_add_library(kanagramengine SHARED ${KanagramEngine_SRCS})
endif()

target_link_libraries(
    kanagramengine
    #${QT_QTXML_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${LIBKDEEDU_KEDUVOCDOCUMENT_LIBRARIES}
)

set(KANAGRAM_ENGINE_LIBRARIES
    kanagramengine
    ${LIBKDEEDU_KEDUVOCDOCUMENT_LIBRARIES}
    CACHE INTERNAL "Libraries required for Kanagram engine"
)

set_target_properties(kanagramengine PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} DEFINE_SYMBOL MAKE_KANAGRAM_ENGINE_LIB)

if(APPLE)
    #hack for being able to set headers as public in a osx framework
    list(APPEND KanagramEngine_HEADERS
        ${KanagramEngine_HEADERS}
    )

    set_target_properties(KanagramEngine PROPERTIES FRAMEWORK TRUE)
    set_target_properties(KanagramEngine PROPERTIES BUILD_WITH_INSTALL_RPATH 1 INSTALL_NAME_DIR "@executable_path/../Frameworks")
    set_target_properties(KanagramEngine PROPERTIES PUBLIC_HEADER "${KanagramEngine_HEADERS}")

    set(MACOSX_FRAMEWORK_IDENTIFIER engine)
    set(MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${GENERIC_LIB_SOVERSION})
    set(MACOSX_FRAMEWORK_BUNDLE_VERSION ${GENERIC_LIB_VERSION})
endif()

install(
    TARGETS     kanagramengine
    RUNTIME     DESTINATION ${BIN_INSTALL_DIR} COMPONENT kanagramengine
    LIBRARY     DESTINATION ${LIB_INSTALL_DIR} COMPONENT kanagramengine
    ARCHIVE     DESTINATION ${LIB_INSTALL_DIR} COMPONENT kanagramengine
    FRAMEWORK   DESTINATION ${LIB_INSTALL_DIR} COMPONENT kanagramengine
)

install(
    FILES
    ${KanagramEngine_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/kanagram
    COMPONENT kanagramengine
)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kanagramsettings.h
    DESTINATION ${INCLUDE_INSTALL_DIR}/kanagram
    COMPONENT kanagramengine
)

install(
    FILES
    kanagram.kcfg
    DESTINATION
    ${KCFG_INSTALL_DIR}
)

install(
    FILES
    kanagram.knsrc
    DESTINATION
    ${CONFIG_INSTALL_DIR}
)

