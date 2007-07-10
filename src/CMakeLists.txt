
include_directories( ${CMAKE_SOURCE_DIR}/libkdeedu/kdeeducore/   )


########### next target ###############

set(kanagram_SRCS 
   main.cpp 
   kanagram.cpp 
   kanagramgame.cpp 
   fontutils.cpp 
   mainsettings.cpp 
   vocabsettings.cpp 
   vocabedit.cpp 
   newstuff.cpp )


kde4_add_ui_files(kanagram_SRCS 
   vocabsettingswidget.ui
   vocabeditwidget.ui
   newstuffwidget.ui
   mainsettingswidget.ui )

kde4_add_kcfg_files(kanagram_SRCS kanagramsettings.kcfgc )

kde4_add_executable(kanagram ${kanagram_SRCS})

target_link_libraries(kanagram  ${KDE4_KNEWSTUFF2_LIBS} ${KDE4_PHONON_LIBS} ${QT_QTXML_LIBRARY} kdeeducore )

install(TARGETS kanagram  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kanagram.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kanagram.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kanagram.knsrc  DESTINATION  ${CONFIG_INSTALL_DIR} )

kde4_install_icons( ${ICON_INSTALL_DIR}   )


