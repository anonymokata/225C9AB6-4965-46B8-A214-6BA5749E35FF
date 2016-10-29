CC=clang-3.5
#CC=gcc
CFLAGS=-c -Wall -ggdb
#Ubuntu -- figure out what flags are needed to run check framework
LIBCHECK_CFG=$(shell pkg-config --cflags --libs check)
#checkmk -- should be included in Ubuntu 'check' package (apt-get install check)
CHECKMK=checkmk

#---source files---
#source files
SOURCES=rpntools.c utils.c errors.c
#objects by renaming sources
OBJECTS=$(SOURCES:.c=.o)

#---tests---
#test location
TESTLOC=./tests
#test .check source files
TESTCHECKS=utils_test.check rpntools_test.check


#---recipes---
all: $(OBJECTS) debug tests

#build objects from .c files
%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

#create .c files from .check files
$(TESTLOC)/%.c:$(TESTLOC)/%.check
	$(CHECKMK) $< > $@

#the test executables require some different compile rules
$(TESTLOC)/utils_test: $(TESTLOC)/utils_test.c $(SOURCES)
	$(CC) -ggdb -o $@ $< utils.c errors.c $(LIBCHECK_CFG)

$(TESTLOC)/rpntools_test: $(TESTLOC)/rpntools_test.c $(SOURCES)
	$(CC) -ggdb -o $@ $< rpntools.c utils.c errors.c $(LIBCHECK_CFG) 

$(TESTLOC)/errors_test: $(TESTLOC)/errors_test.c $(SOURCES)
	$(CC) -ggdb -o $@ $< errors.c $(LIBCHECK_CFG) 

#run the executable which contains the tests
tests: $(TESTLOC)/rpntools_test $(TESTLOC)/utils_test $(TESTLOC)/errors_test
	chmod +x $(TESTLOC)/rpntools_test $(TESTLOC)/utils_test $(TESTLOC)/errors_test
	$(TESTLOC)/rpntools_test
	$(TESTLOC)/utils_test
	$(TESTLOC)/errors_test

debug: debug.c $(SOURCES)
	$(CC) -ggdb debug.c $(SOURCES) -o $@
	chmod +x $@

clean:
	rm -f *.o 
	rm -f $(TESTLOC)/rpntools_test $(TESTLOC)/rpntools_test.c 
	rm -f $(TESTLOC)/utils_test $(TESTLOC)/utils_test.c 
	rm -f $(TESTLOC)/errors_test $(TESTLOC)/errors_test.c
	rm -f debug
