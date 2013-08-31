cmake_minimum_required(VERSION 2.8)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LIBKDEEDU_INCLUDE_DIR}
    ${KANAGRAM_ENGINE_INCLUDE_DIRS}
    ${KDE4_INCLUDES}
)

set(kanagramdesktop_SRCS
    main.cpp
    kanagram.cpp
    mainsettings.cpp
    vocabsettings.cpp
    vocabedit.cpp
)

kde4_add_ui_files(kanagramdesktop_SRCS
    vocabsettingswidget.ui
    vocabeditwidget.ui
    mainsettingswidget.ui
)

# there is no kanagram icon in oxygen
#kde4_add_app_icon(kanagramdesktop_SRCS "${KDE4_ICON_DIR}/oxygen/*/apps/kanagram.png")
kde4_add_app_icon(kanagramdesktop_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../../icons/hi*-app*-kanagram.png")

kde4_add_executable(kanagram ${kanagramdesktop_SRCS})
add_dependencies(kanagram kvtml_data)

target_link_libraries(kanagram
    ${KDE4_KNEWSTUFF3_LIBS}
    ${KDE4_PHONON_LIBS}
    ${QT_QTXML_LIBRARY}
    ${KDE4_KDEUI_LIBS}
    ${LIBKDEEDU_KEDUVOCDOCUMENT_LIBRARIES}
    ${KANAGRAM_ENGINE_LIBRARIES}
)

install(TARGETS kanagram ${INSTALL_TARGETS_DEFAULT_ARGS})

if(NOT APPLE)
    install(PROGRAMS
            kanagram.desktop
            DESTINATION
            ${XDG_APPS_INSTALL_DIR}
    )

    kde4_install_icons(${ICON_INSTALL_DIR})
endif()


