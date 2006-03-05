kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kanagram/sounds FILES right.ogg wrong.ogg chalk.ogg )

kde4_footer()



#original Makefile.am contents follow:

#soundsdir = $(kde_datadir)/kanagram/sounds
#sounds_DATA = right.ogg wrong.ogg chalk.ogg
