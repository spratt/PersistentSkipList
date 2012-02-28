###############################################################################
#                       Copyright (c) 2011 - 2012 by                          #
#                               Simon Pratt                                   #
#                         (All rights reserved)                               #
###############################################################################
# Makefile for the Persistent SkipList project
###############################################################################

###############################################################################
# Global options
###############################################################################

CXX = g++

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

EXECUTABLES 	= ${TEST_TSA} ${TEST_PSL} ${TEST_PS}

#begin actual makefile stuff
all: ${EXECUTABLES}

# specify required libraries
${TEST_TSA}: TimeStampedArray.o

${TEST_PSL}: PersistentSkipList.o

${TEST_PS}: PersistentSkipList.o Point2D.o LineSegment.o PolygonalSubdivision.o

# tidy up generated files
clean:
	@rm -f ${EXECUTABLES}
	@rm -f *.o *.log core
	@rm -rf *.dSYM

