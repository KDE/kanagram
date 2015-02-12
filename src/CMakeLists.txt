set(kanagram_SRCS
    kanagramconfigdialog.cpp
    kanagramgame.cpp
    main.cpp
    mainwindow.cpp
    mainsettings.cpp
    vocabsettings.cpp
    vocabedit.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDES}
    ${LIBKEDUVOCDOCUMENT_INCLUDE_DIR}
)

ki18n_wrap_ui(kanagram_SRCS
    vocabsettingswidget.ui
    vocabeditwidget.ui
    mainsettingswidget.ui
)

kconfig_add_kcfg_files(kanagram_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/kanagramsettings.kcfgc)
# qt5_add_dbus_interfaces(kanagram_SRCS ${KDE4_DBUS_INTERFACES_DIR}/org.kde.KSpeech.xml )
add_executable(kanagram ${kanagram_SRCS})

target_link_libraries(kanagram
    Qt5::Quick
    Qt5::Core
    KF5::CoreAddons
    KF5::I18n
    KF5::NewStuff
    KF5::SonnetCore
    KF5::Declarative
    KF5::Crash
    LibKEduVocDocumentImport__KEduVocDocument
)

if (Qt5TextToSpeech_FOUND)
    target_link_libraries(kanagram
        Qt5::TextToSpeech)
endif()

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
    DIRECTORY ui/
    DESTINATION ${DATA_INSTALL_DIR}/kanagram/ui)

install(
    FILES kanagram.desktop
    DESTINATION ${XDG_APPS_INSTALL_DIR}
)

install(
    TARGETS
    kanagram
    ${INSTALL_TARGETS_DEFAULT_ARGS}
)

