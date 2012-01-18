cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
)

set(KANAGRAM_ENGINE_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LIBKDEEDU_INCLUDE_DIR}
    CACHE INTERNAL "Includes required for Kanagram Engine"
)

set(KanagramEngine_SRCS
    kanagramgame.cpp
)

set(KanagramEngine_HEADERS
    kanagramgame.h
    kanagram_engine_export.h
)

if(BUILD_KANAGRAM_HARMATTAN)
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/harmattan
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/harmattan

        ${LIBKDEEDU_INCLUDE_DIR}
    )

    kde4_add_kcfg_files(KanagramEngine_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/harmattan/kanagramsettings.kcfgc)
endif()

if(BUILD_KANAGRAM_DESKTOP)
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/desktop
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/desktop

        ${LIBKDEEDU_INCLUDE_DIR}
    )

    kde4_add_kcfg_files(KanagramEngine_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/desktop/kanagramsettings.kcfgc)
endif()


if(APPLE)
    kde4_add_library(kanagramengine SHARED ${KanagramEngine_SRCS} ${KanagramEngine_HEADERS})
else()
    kde4_add_library(kanagramengine SHARED ${KanagramEngine_SRCS})
endif()

if(BUILD_KANAGRAM_HARMATTAN)
    target_link_libraries(
        kanagramengine
        #${QT_QTXML_LIBRARY}
        ${KDE4_KDECORE_LIBS}
        ${LIBKDEEDU_KEDUVOCDOCUMENT_LIBRARIES}
    )
endif()

if(BUILD_KANAGRAM_DESKTOP)
    target_link_libraries(
        kanagramengine
        #${QT_QTXML_LIBRARY}
        ${KDE4_KDEUI_LIBS}
        ${LIBKDEEDU_KEDUVOCDOCUMENT_LIBRARIES}
    )
endif()

set(KANAGRAM_ENGINE_LIBRARIES
    kanagramengine
    ${LIBKDEEDU_KEDUVOCDOCUMENT_LIBRARIES}
    CACHE INTERNAL "Libraries required for Kanagram engine"
)

set_target_properties(kanagramengine PROPERTIES VERSION ${KANAGRAM_VERSION_STRING} SOVERSION ${KANAGRAM_VERSION_STRING} DEFINE_SYMBOL MAKE_KANAGRAM_ENGINE_LIB)

if(APPLE)
    #hack for being able to set headers as public in a osx framework
    list(APPEND KanagramEngine_HEADERS
        ${KanagramEngine_HEADERS}
    )

    set_target_properties(KanagramEngine PROPERTIES FRAMEWORK TRUE)
    set_target_properties(KanagramEngine PROPERTIES BUILD_WITH_INSTALL_RPATH 1 INSTALL_NAME_DIR "@executable_path/../Frameworks")
    set_target_properties(KanagramEngine PROPERTIES PUBLIC_HEADER "${KanagramEngine_HEADERS}")

    set(MACOSX_FRAMEWORK_IDENTIFIER engine)
    set(MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${KANAGRAM_VERSION_STRING})
    set(MACOSX_FRAMEWORK_BUNDLE_VERSION ${KANAGRAM_VERSION_STRING})
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

if(BUILD_KANAGRAM_HARMATTAN)
    install(
        FILES
        harmattan/kanagram.kcfg
        DESTINATION
        ${KCFG_INSTALL_DIR}
    )
endif()

if(BUILD_KANAGRAM_DESKTOP)
    install(
        FILES
        desktop/kanagram.kcfg
        DESTINATION
        ${KCFG_INSTALL_DIR}
    )
endif()

install(
    FILES
    kanagram.knsrc
    DESTINATION
    ${CONFIG_INSTALL_DIR}
)

