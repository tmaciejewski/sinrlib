LDADD = -lcairo
AM_CXXFLAGS= -O2 -Wall -g -pg
AM_CPPFLAGS= -I/usr/include/cairo

bin_PROGRAMS = sinr show_model generate_model

sinr_SOURCES = backoffack.cpp backoffack.h backoff.cpp backoff.h densityknown.cpp \
			   densityknown.h densityunknown.cpp densityunknown.h gadget.cpp gadget.h \
			   model.cpp model.h naive.cpp naive.h simulation.cpp simulation.h sinr.cpp \
			   sinrlib.h social.cpp social.h uniform.cpp uniform.h antibackoff.h antibackoff.cpp

show_model_SOURCES = model.cpp model.h show_model.cpp sinrlib.h

generate_model_SOURCES = gadget.cpp gadget.h model.cpp model.h \
						 generate_model.cpp sinrlib.h social.cpp \
						 social.h uniform.cpp uniform.h
