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
test_list: control_utilities.o array_utilities.o PersistentList.o PointPersistentList.o test_list.cpp

test_timestamped_array: TimeStampedArray.o test_timestamped_array.cpp

# object files
PersistentList.o: array_utilities.o PersistentList.h

PointPersistentList.o: array_utilities.o sort/heap_sort.o PersistentList.o PointPersistentList.h

sort/heap_sort.o: sort/heap_sort.h

array_utilities.o: array_utilities.h

control_utilities.o: control_utilities.h

TimeStampedArray.o: TimeStampedArray.h

# in case of emergency
clean:
	@if [ -f test_list ]; then rm test_list; fi
	@rm -f *.o # remove all object files and be quiet about it

