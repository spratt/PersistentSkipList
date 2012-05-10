###############################################################################
#                       Copyright (c) 2011 - 2012 by                          #
#                               Simon Pratt                                   #
#                         (All rights reserved)                               #
###############################################################################
# Makefile for the Persistent SkipList project                                #
###############################################################################

# don't use built in targets
makepp_no_builtin = 1

###############################################################################
# Quiet Compile                                                               #
###############################################################################
QUIET_CC	=
# comment next line for verbose compile
QUIET_CC	= @echo '    ' CC $@;

%.o: %.cpp
	$(QUIET_CC)$(COMPILE.cpp) $(OUTPUT_OPTION) $<

%: %.cpp
	$(QUIET_CC)$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

###############################################################################
# Global options                                                              #
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

BAR = "======================================================================"

###############################################################################
# Project specific options                                                    #
###############################################################################

TEST_TSA	= test/test_timestamped_array
TEST_LN		= test/test_psl_listnode
TEST_PSL	= test/test_persistent_skiplist
TEST_PS		= test/test_polygonal_subdivision

TESTS	 	= ${TEST_TSA} ${TEST_LN} ${TEST_PSL} ${TEST_PS}

.PHONY:	all run run_tests_mac run_tests clean lines

.IGNORE: lines

.SUFFIXES: .o .cpp .hpp

.SILENT: run_tests run_tests_mac

#begin actual makefile stuff
all: ${TESTS}

run: run_tests_mac

test: run_tests_mac run_tests

run_tests_mac:	all
	${foreach test,${TESTS},\
echo ${BAR};echo "| " ${test};echo ${BAR};\
cat ${test}_input | ./${test};}

run_tests:	all	
	${foreach test,${TESTS},\
echo ${BAR};echo ${test};echo ${BAR};\
cat ${test}_input | ${VALGRIND} ${VGOPS} ./${test};}

# specify required libraries
${TEST_TSA}: 	TimeStampedArray.o

${TEST_LN}:  	Point2D.o LineSegment.o PersistentSkipList.o \
	     	lib/SmartPointer/SmartPointer.o

${TEST_PSL}: 	Point2D.o LineSegment.o PersistentSkipList.o \
	     	lib/SmartPointer/SmartPointer.o

${TEST_PS}: 	Point2D.o LineSegment.o PersistentSkipList.o PolygonalSubdivision.o

# tidy up generated files
clean:
	@rm -f ${TESTS}
	@rm -f *.o *.log core
	@rm -rf *.dSYM

###############################################################################
# Miscellaneous                                                               #
###############################################################################

lines:
	@wc -l *.c *.h *.hpp *.cpp 2> /dev/null || \
	sloccount *.cpp *.c *.h 2> /dev/null | grep Physical || \
	cloc --by-file *.cpp *.c *.hpp *.h 2> /dev/null