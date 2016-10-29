CC=clang-3.5
#CC=gcc
CFLAGS=-c -Wall -ggdb
#Ubuntu -- figure out what flags are needed to run check framework
LIBCHECK_CFG=$(shell pkg-config --cflags --libs check)
#checkmk -- should be included in Ubuntu 'check' package (apt-get install check)
CHECKMK=checkmk
#source files
SOURCES=rpntools.c
#objects by renaming sources
OBJECTS=$(SOURCES:.c=.o)
#test location
TESTLOC=./tests


all: $(OBJECTS) debug tests

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

#run the executable which contains the tests
tests: $(TESTLOC)/rpnstring_test
	chmod +x $(TESTLOC)/rpnstring_test
	$(TESTLOC)/rpnstring_test

debug: debug.c $(SOURCES)
	$(CC) -ggdb debug.c rpntools.c -o $@
	chmod +x $@

#generate test.c from check file
$(TESTLOC)/rpnstring_test.c: $(TESTLOC)/rpnstring_test.check 
	$(CHECKMK) $< > $@

#the test executable requires some different compile rules
$(TESTLOC)/rpnstring_test: $(TESTLOC)/rpnstring_test.c rpntools.c rpntools.h
	$(CC) -ggdb -o $@ $< rpntools.c $(LIBCHECK_CFG) 

clean:
	rm -f *.o $(TESTLOC)/rpnstring_test $(TESTLOC)/rpnstring_test.c debug
