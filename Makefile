# Vorbital makefile.  Requires wxWidgets.
# by default, wx-config from the PATH is used
WX_CONFIG := /usr/bin/wx-config

# Main executable file
PROGRAM = SpaceTheremin

# Object files
OBJECTS = spacethereminapp.o spacetheremindlg.o Wavetable.o

CXX = $(shell $(WX_CONFIG) --cxx -ggdb)

.SUFFIXES:	.o .cpp

.cpp.o :
	$(CXX) -ggdb -c `$(WX_CONFIG) --cxxflags` -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) `$(WX_CONFIG) --libs` `pkg-config --cflags --libs jack`

clean: 
	rm -f *.o $(PROGRAM)
