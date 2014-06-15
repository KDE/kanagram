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

set(KANAGRAM_ENGINE_INCLUDE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LIBKDEEDU_INCLUDE_DIR}
    CACHE INTERNAL "Includes required for Kanagram Engine"
)

set(kanagram-active_SRCS
    kanagramgame.cpp
    kanagramenginehelper.cpp
    kdeclarativemainwindow.cpp
    kdeclarativeview.cpp
    main.cpp
    mainwindow.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
    ${LIBKDEEDU_INCLUDE_DIR}
)

kde4_add_kcfg_files(kanagram-active_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/kanagramsettings.kcfgc)
kde4_add_executable(kanagram-active ${kanagram-active_SRCS})

target_link_libraries(kanagram-active
    ${QT_QTDECLARATIVE_LIBRARY}
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_PLASMA_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${LIBKDEEDU_KEDUVOCDOCUMENT_LIBRARIES}
    ${KANAGRAM_ENGINE_LIBRARIES}
    kdeclarative
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

install(
    DIRECTORY package/
    DESTINATION ${DATA_INSTALL_DIR}/plasma/packages/org.kde.kanagram)

install(
    FILES kanagram-active.desktop
    DESTINATION ${XDG_APPS_INSTALL_DIR}
)

install(
    TARGETS
    kanagram-active
    ${INSTALL_TARGETS_DEFAULT_ARGS}
)

