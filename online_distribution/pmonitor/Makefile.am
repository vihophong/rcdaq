AUTOMAKE_OPTIONS = foreign

PACKAGE = pmonitor

include_HEADERS = \
  pmonitor.h \
  pmonstate.h \
  pmondisplay.h \
  pmongui.h \
  pMutex.h \
  MonitorServer.h

LINKFILE = pmonitorLinkDef.h


noinst_HEADERS = $(LINKFILE)

BUILT_SOURCES = pmonitor_dict.C  MonitorClient_dict.C

#ROOTCINT =  $(ROOTSYS)/bin/rootcint 
ROOTCINT =  rootcint 

INCLUDES =  -I$(includedir) -I$(ONLINE_MAIN)/include -I@ROOTINC@

lib_LTLIBRARIES = libpmonitor.la libMonitorClient.la

LDFLAGS = -Wl,--no-as-needed 


libpmonitor_la_SOURCES = \
  pmonitor.cc \
  pmongui.cc \
  MonitorServer.cc \
  pmonitor.h \
  pmonstate.h \
  pmondisplay.h \
  pmongui.h \
  pMutex.cc \
  pmonitor_dict.C

libMonitorClient_la_SOURCES = \
  MonitorClient.cc \
  MonitorClient.h \
  MonitorClient_dict.C


if FROG

libpmonitor_la_LIBADD = \
  -L$(libdir) \
  -L$(ONLINE_MAIN)/lib \
  -lEvent -lmessage \
  $(ROOTLIBS) -lThread -lpthread \
  -L$(OFFLINE_MAIN)/lib 

else


libpmonitor_la_LIBADD = \
  -L$(libdir) \
  -L$(ONLINE_MAIN)/lib \
  -lEvent -lmessage \
  $(ROOTLIBS) -lThread -lpthread 

endif

libMonitorClient_la_LIBADD = \
  $(ROOTLIBS) 



pmonitor_dict.C: pmonitor.h pmonstate.h pmondisplay.h pmongui.h $(LINKFILE)
	$(ROOTCINT) -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

MonitorClient_dict.C: MonitorClient.h MonitorClientLinkDef.h
	$(ROOTCINT) -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^


bin_SCRIPTS = writePmonProject.pl

clean-local:
	rm *_dict.*
