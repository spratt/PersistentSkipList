# if mode is release, don't include debug info
ifeq ($(mode),release)
	CXXFLAGS=-O2 -Wall
else
	mode = debug
	CXXFLAGS=-O0 -g -Wall
endif

#begin actual makefile stuff
all: test_list

# executables
test_list: PersistentList.o test_list.cpp

# object files
PersistentList.o: PersistentList.h

# in case of emergency
clean:
	@if [ -f test_list ]; then rm test_list; fi
	@rm -f *.o # remove all object files and be quiet about it

