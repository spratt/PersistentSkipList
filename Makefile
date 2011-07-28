# if mode is release, don't include debug info
ifeq ($(mode),release)
	CXXFLAGS=-O2 -Wall
else
	mode = debug
	CXXFLAGS=-g -Wall
endif

#begin actual makefile stuff
all: test_list

# executables
test_persistent_skiplist: PersistentSkipList.o test_persistent_skiplist.cpp

test_timestamped_array: TimeStampedArray.o test_timestamped_array.cpp

# object files
PersistentSkipList.o: PersistentSkipList.h

sort/heap_sort.o: sort/heap_sort.h

array_utilities.o: array_utilities.h

control_utilities.o: control_utilities.h

TimeStampedArray.o: TimeStampedArray.h

# in case of emergency
clean:
	@if [ -f test_list ]; then rm test_list; fi
	@rm -f *.o # remove all object files and be quiet about it

