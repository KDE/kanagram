kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kanagram/data/en FILES objects.kvtml worldcapitals.kvtml computers.kvtml animals.kvtml sports.kvtml people.kvtml inventions.kvtml currencies.kvtml fruits.kvtml vegetables.kvtml space.kvtml professions.kvtml clothing.kvtml transportation.kvtml numbers.kvtml )

kde4_footer()



#original Makefile.am contents follow:

#txt_DATA = objects.kvtml worldcapitals.kvtml computers.kvtml animals.kvtml sports.kvtml people.kvtml inventions.kvtml currencies.kvtml fruits.kvtml vegetables.kvtml space.kvtml professions.kvtml clothing.kvtml transportation.kvtml numbers.kvtml
#txtdir = $(kde_datadir)/kanagram/data/en
