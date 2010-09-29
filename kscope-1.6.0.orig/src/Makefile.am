# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = bookmarksdlg.h calltreedlg.h calltreemanager.h \
	configfrontend.h cscopefrontend.h cscopemsgdlg.h ctagsfrontend.h ctagslist.h \
	dirscanner.h dotfrontend.h editormanager.h editorpage.h editortabs.h encoder.h \
	filelist.h fileview.h frontend.h graphedge.h graphnode.h graphprefdlg.h \
	graphwidget.h historypage.h historyview.h kscope.h kscopeactions.h kscopeconfig.h \
	kscopepixmaps.h makedlg.h makefrontend.h newprojectdlg.h openprojectdlg.h prefcolor.h \
	preferencesdlg.h preffont.h preffrontend.h prefopt.h progressdlg.h project.h \
	projectbase.h projectfilesdlg.h projectmanager.h querypage.h querypagebase.h \
	queryresultsmenu.h queryview.h queryviewdlg.h queryviewdriver.h querywidget.h \
	scanprogressdlg.h searchlist.h searchresultsdlg.h symbolcompletion.h symboldlg.h \
	tabwidget.h treewidget.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kscope.pot

KDE_ICON = kscope

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kscope

# the application source, library search path, and link libraries
kscope_SOURCES = autocompletionlayout.ui bookmarksdlg.cpp bookmarkslayout.ui \
	calltreedlg.cpp calltreelayout.ui calltreemanager.cpp configfrontend.cpp \
	cscopefrontend.cpp cscopemsgdlg.cpp cscopemsglayout.ui ctagsfrontend.cpp ctagslist.cpp \
	dirscanner.cpp dotfrontend.cpp dotparse.ypp dotscan.lpp editormanager.cpp \
	editorpage.cpp editortabs.cpp encoder.cpp filelist.cpp fileview.cpp fileviewlayout.ui \
	frontend.cpp graphedge.cpp graphnode.cpp graphprefdlg.cpp graphpreflayout.ui \
	graphwidget.cpp historypage.cpp historyview.cpp kscope.cpp kscopeactions.cpp \
	kscopeconfig.cpp kscopepixmaps.cpp main.cpp makedlg.cpp makefrontend.cpp makelayout.ui \
	newprojectdlg.cpp newprojectlayout.ui openprojectdlg.cpp openprojectlayout.ui \
	prefcolor.cpp prefcolorlayout.ui preferencesdlg.cpp preffont.cpp preffontlayout.ui \
	preffrontend.cpp preffrontendlayout.ui prefopt.cpp prefoptlayout.ui progressdlg.cpp \
	project.cpp projectbase.cpp projectfilesdlg.cpp projectfileslayout.ui \
	projectmanager.cpp querypage.cpp querypagebase.cpp queryresultsmenu.cpp queryview.cpp \
	queryviewdlg.cpp queryviewdriver.cpp queryviewlayout.ui querywidget.cpp \
	querywidgetlayout.ui scanprogressdlg.cpp scanprogresslayout.ui searchlist.cpp \
	searchresultsdlg.cpp searchresultslayout.ui symbolcompletion.cpp symboldlg.cpp \
	symbollayout.ui tabwidget.cpp treewidget.cpp welcomedlg.ui

kscope_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kscope_LDADD = -lkateinterfaces -lktexteditor $(LIB_KDEUI)

# this is where the desktop file will go 
shelldesktopdir = $(kde_appsdir)/Development
shelldesktop_DATA = kscope.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir = $(kde_datadir)/kscope
shellrc_DATA = kscopeui.rc kscope_config

picsdir = $(kde_datadir)/kscope/pics
pics_DATA = file_ro.png file_rw.png file_save.png query_locked.png \
			query_unlocked.png tab_list.png call_graph.png called_tree.png calling_tree.png \
			bookmark.png

BUILT_SOURCES = dotparse.h
AM_YFLAGS = -d
