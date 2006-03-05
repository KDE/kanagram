kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


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

kde4_automoc(${kanagram_SRCS})

kde4_add_ui_files(kanagram_SRCS 
   mainsettingswidget.ui 
   vocabsettingswidget.ui 
   vocabeditwidget.ui 
   newstuffwidget.ui )

kde4_add_kcfg_files(kanagram_SRCS kanagramsettings.kcfgc )

kde4_add_executable(kanagram ${kanagram_SRCS})

target_link_libraries(kanagram  ${QT_AND_KDECORE_LIBS} kdeui knewstuff kio kdeeducore )

install_targets(/bin kanagram )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kanagram.desktop )
install_files( ${KCFG_INSTALL_DIR} FILES kanagram.kcfg )

kde4_install_icons( ${ICON_INSTALL_DIR}  hicolor )

kde4_footer()



#original Makefile.am contents follow:

#if include_ARTS
#artslib = -lartskde
#else
#artslib =
#endif
#
## set the include path for X, qt and KDE
#INCLUDES = $(all_includes) -I$(srcdir)/../../libkdeedu/kdeeducore/
#
## these are the headers for your project
#noinst_HEADERS = kanagram.h kanagramgame.h fontutils.h vocabsettings.h vocabedit.h vocabsettingswidget.h vocabeditwidget.h mainsettingswidget.h mainsettings.h newstuffwidget.h newstuffdialog.h newstuff.h
#
## let automoc handle all of the meta source files (moc)
#METASOURCES = AUTO
#
#messages: rc.cpp
#	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` >> rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/kanagram.pot
#
#KDE_ICON = AUTO
#
##########################################################################
## APPLICATION SECTION
##########################################################################
## this is the program that gets installed.  it's name is used for all
## of the other Makefile.am variables
#bin_PROGRAMS = kanagram
#
## the application source, library search path, and link libraries
#kanagram_SOURCES = main.cpp kanagram.cpp kanagramgame.cpp fontutils.cpp kanagramsettings.kcfgc mainsettingswidget.ui vocabsettingswidget.ui vocabeditwidget.ui mainsettings.cpp vocabsettings.cpp vocabedit.cpp newstuffwidget.ui newstuffdialog.cpp newstuff.cpp
#kanagram_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#kanagram_LDADD =  $(LIB_KDEUI) $(LIB_KNEWSTUFF) $(LIB_KIO) $(artslib) ../../libkdeedu/kdeeducore/libkdeeducore.la
#
#kde_kcfg_DATA=kanagram.kcfg
#
#
## this is where the desktop file will go 
#xdg_apps_DATA = kanagram.desktop
#
## this is where the shell's XML-GUI resource file goes
## TODO does this app need an ui.rc ? If it does put it here
## shellrcdir   = $(kde_datadir)/kanagram
## shellrc_DATA = kanagramui.rc
