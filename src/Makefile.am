if include_ARTS
artslib = -lartskde
else
artslib =
endif

# set the include path for X, qt and KDE
INCLUDES = $(all_includes) -I$(srcdir)/../../libkdeedu/kdeeducore/

# these are the headers for your project
noinst_HEADERS = kanagram.h kanagramgame.h fontutils.h vocabsettings.h vocabedit.h vocabsettingswidget.h vocabeditwidget.h mainsettingswidget.h mainsettings.h newstuffwidget.h newstuffdialog.h newstuff.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO


KDE_ICON = AUTO

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kanagram

# the application source, library search path, and link libraries
kanagram_SOURCES = main.cpp kanagram.cpp kanagramgame.cpp fontutils.cpp kanagramsettings.kcfgc mainsettingswidget.ui vocabsettingswidget.ui vocabeditwidget.ui mainsettings.cpp vocabsettings.cpp vocabedit.cpp newstuffwidget.ui newstuffdialog.cpp newstuff.cpp
kanagram_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kanagram_LDADD =  $(LIB_KDEUI) $(LIB_KNEWSTUFF) $(LIB_KIO) $(artslib) ../../libkdeedu/kdeeducore/libkdeeducore.la

kde_kcfg_DATA=kanagram.kcfg


# this is where the desktop file will go 
xdg_apps_DATA = kanagram.desktop

# this is where the shell's XML-GUI resource file goes
# TODO does this app need an ui.rc ? If it does put it here
# shellrcdir   = $(kde_datadir)/kanagram
# shellrc_DATA = kanagramui.rc
