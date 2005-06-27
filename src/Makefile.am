# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = kanagram.h kanagramgame.h fontutils.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kanagram.pot

KDE_ICON = AUTO

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kanagram

# the application source, library search path, and link libraries
kanagram_SOURCES = main.cpp kanagram.cpp kanagramgame.cpp fontutils.cpp inputbox.cpp
kanagram_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#TODO -libkdeeducore will have to be changed to ../../libkdeedu/kdeeducore/libkdeeducore.la when kanagram gets in kdeedu module
kanagram_LDADD =  $(LIB_KDEUI) -lkdeeducore


# this is where the desktop file will go 
xdg_apps_DATA = kanagram.desktop

# this is where the shell's XML-GUI resource file goes
# TODO does this app need an ui.rc ? If it does put it here
# shellrcdir   = $(kde_datadir)/kanagram
# shellrc_DATA = kanagramui.rc

