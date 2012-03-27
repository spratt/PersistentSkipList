###############################################################################
#                       Copyright (c) 2011 - 2012 by                          #
#                               Simon Pratt                                   #
#                         (All rights reserved)                               #
###############################################################################
# Makefile for the Persistent SkipList project
###############################################################################

###############################################################################
# Quiet Compile
###############################################################################
QUIET_CC	=
# comment next line for verbose compile
QUIET_CC	= @echo '    ' CC $@;

makepp_no_builtin = 1

%.o: %.cpp
	$(QUIET_CC)$(COMPILE.cpp) $(OUTPUT_OPTION) $<

%: %.cpp
	$(QUIET_CC)$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

###############################################################################
# Global options
###############################################################################

CXX = g++
CXXFLAGS       	= -g -std=c++98 -pedantic-errors -Wall -Werror

VALGRIND	= valgrind
VGOPS		= --leak-check=full -v --show-reachable=yes

# if mode is release, don't include debug info
ifeq ($(mode),release)
	CXXFLAGS=-O2 -Wall
else
	mode = debug
	CXXFLAGS=-g -std=c++98 -pedantic-errors -Wall -Werror
endif

###############################################################################
# Project specific options
###############################################################################

TEST_TSA	= test_timestamped_array
TEST_PSL	= test_persistent_skiplist
TEST_PS		= test_polygonal_subdivision

TESTS	 	= ${TEST_TSA} ${TEST_PSL} ${TEST_PS}

#begin actual makefile stuff
all: ${TESTS}

run: all run_tests

run_tests_mac:	all
	${foreach test,${TESTS},cat ${test}_input | ./${test};}

run_tests:	run_tests_mac
	${foreach test,${TESTS},cat ${test}_input | ${VALGRIND} ${VGOPS} ./${test};}

# specify required libraries
${TEST_TSA}: TimeStampedArray.o

${TEST_PSL}: PersistentSkipList.o

${TEST_PS}: PersistentSkipList.o Point2D.o LineSegment.o PolygonalSubdivision.o

# tidy up generated files
clean:
	@rm -f ${TESTS}
	@rm -f *.o *.log core
	@rm -rf *.dSYM

######################################################################
# MISC
######################################################################

lines:
	@wc -l *.c *.h *.cpp 2> /dev/null || \
	sloccount *.cpp *.c *.h 2> /dev/null | grep Physical
	cloc *.cpp *.c *.h 2> /dev/null
	cloc --by-file *.cpp *.c *.h 2> /dev/null