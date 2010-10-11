# Define executable name
BIN = sim 


# Define source files
SRCS = simulation.cpp mersenne.cpp stoc1.cpp userintf.cpp

# Define header file paths
INCPATH = headers/*.h

# Define the -L library path(s)
LDFLAGS =

# Define the -l library name(s)
LIBS =

# Only in special cases should anything be edited below this line
#
OBJS      = $(SRCS:.cpp=.o)
CXXFLAGS  = -Wall -g -O0
DEP_FILE  = .depend


.PHONY = all clean distclean


# Main entry point
#
all: depend $(BIN)


# For linking object file(s) to produce the executable
#
$(BIN): $(OBJS)
	@echo Linking $@
	@$(CXX) $^ $(LDFLAGS) $(LIBS) -o $@


# For compiling source file(s)
#
.cpp.o:
	@echo Compiling $<
	@$(CXX) -c $(CXXFLAGS) $(INCPATH) $<


# For cleaning up the project
#
clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(BIN)
	$(RM) $(DEP_FILE)


# For determining source file dependencies
#
depend: $(DEP_FILE)
	@touch $(DEP_FILE)

$(DEP_FILE):
	@echo Generating dependencies in $@
	@-$(CXX) -E -MM $(CXXFLAGS) $(INCPATH) $(SRCS) >> $(DEP_FILE)








ifeq (,$(findstring clean,$(MAKECMDGOALS)))
ifeq (,$(findstring distclean,$(MAKECMDGOALS)))
-include $(DEP_FILE)
endif
endif

