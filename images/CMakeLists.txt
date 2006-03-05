kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kanagram/images FILES kanagram.png hint.png eyes1.png eyes2.png eyes3.png eyes4.png eyes5.png eyes6.png eyes7.png eyes8.png kicon.png kiconover.png appicon.png appiconover.png handbookiconover.png handbookicon.png card.png arrow.png arrowover.png basicarrow.png basicarrowover.png next.png nextover.png help.png helpover.png quit.png quitover.png config.png configover.png up.png upover.png updisabled.png )

kde4_footer()



#original Makefile.am contents follow:

#imagesdir = $(kde_datadir)/kanagram/images
#images_DATA = kanagram.png hint.png eyes1.png eyes2.png eyes3.png eyes4.png eyes5.png eyes6.png eyes7.png eyes8.png kicon.png kiconover.png appicon.png appiconover.png handbookiconover.png handbookicon.png card.png arrow.png arrowover.png basicarrow.png basicarrowover.png next.png nextover.png help.png helpover.png quit.png quitover.png config.png configover.png up.png upover.png updisabled.png
