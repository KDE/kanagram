
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
   newstuffdialog.cpp 
   newstuff.cpp )

kde4_automoc(kanagram ${kanagram_SRCS})

kde4_add_ui_files(kanagram_SRCS 
   vocabsettingswidget.ui
   vocabeditwidget.ui
   newstuffwidget.ui
   mainsettingswidget.ui )

kde4_add_kcfg_files(kanagram kanagram_SRCS kanagramsettings.kcfgc )

kde4_add_executable(kanagram ${kanagram_SRCS})

target_link_libraries(kanagram  ${KDE4_KNEWSTUFF_LIBS} ${KDE4_KIO_LIBS} kdeeducore )

install(TARGETS kanagram  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kanagram.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES kanagram.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )

kde4_install_icons( ${ICON_INSTALL_DIR}   )


