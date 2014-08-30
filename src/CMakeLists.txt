set(kanagram_SRCS
    kanagramgame.cpp
    kanagramenginehelper.cpp
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
    ${PHONON_INCLUDES}
)

kde4_add_ui_files(kanagram_SRCS
    vocabsettingswidget.ui
    vocabeditwidget.ui
    mainsettingswidget.ui
)

kde4_add_kcfg_files(kanagram_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/kanagramsettings.kcfgc)
# qt5_add_dbus_interfaces(kanagram_SRCS ${KDE4_DBUS_INTERFACES_DIR}/org.kde.KSpeech.xml )
add_executable(kanagram ${kanagram_SRCS})

target_link_libraries(kanagram
    Qt5::Quick
    Qt5::Core
    Qt5::WebKit
    KF5::I18n
    KF5::NewStuff
    KF5::SonnetCore
    KF5::KDELibs4Support
    KF5::Declarative
    LibKEduVocDocumentImport__KEduVocDocument
    Phonon::phonon4qt5
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

