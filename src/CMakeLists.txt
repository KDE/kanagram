ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX KANAGRAM VERSION_HEADER kanagram_version.h)

set(kanagram_SRCS
    kanagramconfigdialog.cpp
    kanagramgame.cpp
    main.cpp
    mainwindow.cpp
    mainsettings.cpp
    vocabsettings.cpp
    vocabedit.cpp
    kanagramconfigdialog.h
    kanagramgame.h
    mainwindow.h
    mainsettings.h
    vocabsettings.h
    vocabedit.h
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

ki18n_wrap_ui(kanagram_SRCS
    vocabsettingswidget.ui
    vocabeditwidget.ui
    mainsettingswidget.ui
)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/*-apps-kanagram.png")
ecm_add_app_icon(cantor_SRCS ICONS ${ICONS_SRCS})


kconfig_add_kcfg_files(kanagram_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/kanagramsettings.kcfgc)
# qt5_add_dbus_interfaces(kanagram_SRCS ${KDE4_DBUS_INTERFACES_DIR}/org.kde.KSpeech.xml )
ecm_add_app_icon(kanagram_SRCS ICONS
                ${CMAKE_SOURCE_DIR}/icons/16-apps-kanagram.png
                ${CMAKE_SOURCE_DIR}/icons/32-apps-kanagram.png
                ${CMAKE_SOURCE_DIR}/icons/64-apps-kanagram.png
                ${CMAKE_SOURCE_DIR}/icons/128-app-kanagram.png
)
add_executable(kanagram ${kanagram_SRCS})

target_link_libraries(kanagram
    Qt::Quick
    Qt::Core
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::NewStuffWidgets
    KF${KF_MAJOR_VERSION}::SonnetCore
    KF${KF_MAJOR_VERSION}::Crash
    KF${KF_MAJOR_VERSION}::ConfigCore
    KF${KF_MAJOR_VERSION}::ConfigWidgets
    KF${KF_MAJOR_VERSION}::XmlGui
    LibKEduVocDocumentImport__KEduVocDocument
)

if (Qt${QT_MAJOR_VERSION}TextToSpeech_FOUND)
    target_link_libraries(kanagram
        Qt${QT_MAJOR_VERSION}::TextToSpeech)
endif()

install(
    FILES
    kanagram.kcfg
    DESTINATION
    ${KDE_INSTALL_KCFGDIR}
)

install(
    FILES
    kanagram.knsrc
    DESTINATION
    ${KDE_INSTALL_KNSRCDIR}
)

install(
    DIRECTORY ui/
    DESTINATION ${KDE_INSTALL_DATADIR}/kanagram/ui)

install(
    FILES org.kde.kanagram.desktop
    DESTINATION ${KDE_INSTALL_APPDIR}
)

install(
    TARGETS
    kanagram
    ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

