#CC=clang-3.5
CC=gcc
CFLAGS=-c -Wall
#Ubuntu -- figure out what flags are needed to run check framework
LIBCHECK_CFG=$(shell pkg-config --cflags --libs check)
#checkmk -- should be included in Ubuntu 'check' package (apt-get install check)
CHECKMK=checkmk
#source files
SOURCES=rpnstring.c util.c
#objects by renaming sources
OBJECTS=$(SOURCES:.c=.o)
#test location
TESTLOC=./tests


all: $(OBJECTS) test

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

#run the executable which contains the tests
test: $(TESTLOC)/rpnstring_test
	chmod +x $(TESTLOC)/rpnstring_test
	$(TESTLOC)/rpnstring_test

$(TESTLOC)/rpnstring_test.c: $(TESTLOC)/rpnstring_test.check
	$(CHECKMK) $< > $@

#the test executable requires some different compile rules
$(TESTLOC)/rpnstring_test: $(TESTLOC)/rpnstring_test.c
	$(CC) -o $@ $< util.c $(LIBCHECK_CFG) 

clean:
	rm *.o $(TESTLOC)/rpnstring_test $(TESTLOC)/rpnstring_test.c
