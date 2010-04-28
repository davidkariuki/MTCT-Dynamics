CC = g++
CFLAGS = -Wall -g -O0 
LDFLAGS = 
HEADERS = outputs.h deaths.h transmission.h person.h arvs.h virus.h rand/randomc.h rand/stocc.h utils.h inputs.h
SOURCES = simulation.cpp rand/mersenne.cpp rand/stoc1.cpp rand/userintf.cpp
OBJECTS = $(SOURCES:.c=.o)
LIBRARIES =  
TARGET = sim

default: $(TARGET)

$(TARGET) : $(OBJECTS) Makefile.dependencies
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBRARIES)

Makefile.dependencies:: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -MM $(SOURCES) > Makefile.dependencies

-include Makefile.dependencies

# The phony target "clean" that is used to remove all compiled object files.

.PHONY: clean

clean:
	@rm -fr $(TARGET) Makefile.dependencies
