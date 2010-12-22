
include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/keduvocdocument/   )


########### next target ###############

set(kanagram_SRCS
   main.cpp
   kanagram.cpp
   kanagramgame.cpp
   mainsettings.cpp
   vocabsettings.cpp
   vocabedit.cpp )


kde4_add_ui_files(kanagram_SRCS
   vocabsettingswidget.ui
   vocabeditwidget.ui
   mainsettingswidget.ui )

# there is no kanagram icon in oxgen
#kde4_add_app_icon(kanagram_SRCS "${KDE4_ICON_DIR}/oxygen/*/apps/kanagram.png")
kde4_add_app_icon(kanagram_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app*-kanagram.png")

kde4_add_kcfg_files(kanagram_SRCS kanagramsettings.kcfgc )

kde4_add_executable(kanagram ${kanagram_SRCS})
add_dependencies(kanagram kvtml_data)

target_link_libraries(kanagram  ${KDE4_KNEWSTUFF3_LIBS} ${KDE4_PHONON_LIBS} ${QT_QTXML_LIBRARY} keduvocdocument kdeeduui )

install(TARGETS kanagram  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS kanagram.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES kanagram.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kanagram.knsrc  DESTINATION  ${CONFIG_INSTALL_DIR} )

kde4_install_icons( ${ICON_INSTALL_DIR}   )


