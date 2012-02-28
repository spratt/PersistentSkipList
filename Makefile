# if mode is release, don't include debug info
ifeq ($(mode),release)
	CXXFLAGS=-O2 -Wall
else
	mode = debug
	CXXFLAGS=-g -Wall
endif

#begin actual makefile stuff
all: test_timestamped_array test_persistent_skiplist

# executables
test_persistent_skiplist: PersistentSkipList.o test_persistent_skiplist.cpp

test_timestamped_array: TimeStampedArray.o test_timestamped_array.cpp

# object files
PersistentSkipList.o: PersistentSkipList.hpp

sort/heap_sort.o: sort/heap_sort.hpp

array_utilities.o: array_utilities.hpp

control_utilities.o: control_utilities.hpp

TimeStampedArray.o: TimeStampedArray.hpp

Point2D.o: Point2D.hpp

# in case of emergency
clean:
	@rm -f test_timestamped_array test_persistent_skiplist
	@rm -f *.o *.log core
	@rm -rf *.dSYM

