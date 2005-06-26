# set the include path for X, qt and KDE
INCLUDES = -I/usr/include/libkdeedu/kdeeducore $(all_includes)

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
kanagram_SOURCES = main.cpp kanagram.cpp kanagramgame.cpp fontutils.cpp
kanagram_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kanagram_LDADD =  $(LIB_KDEUI) /usr/lib/libkdeeducore.la


# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = kanagram.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/kanagram
shellrc_DATA = kanagramui.rc

