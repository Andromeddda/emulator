#----------
# Compiler
#----------

CC = g++

#-------------
# Directories
#-------------

INCLUDES = includes
SRCDIR = src
BUILD = build
DEPDIR := $(BUILD)/deps
PROGDIR = programs

#-------
# Flags
#-------

CFLAGS = \
	-Wall \
	-Werror \
	-Wextra \
	-std=c++20 \
	-O3

# Add include directory
CFLAGS += -I $(abspath $(INCLUDES))

# Ask compiler for dependencies
DEPFLAGS = \
	-MT $@ \
	-MMD \
	-MP \
	-MF $(DEPDIR)/$*.Td

# move temporary depenendecy files to .d files
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

#--------------------------
# Colors (ANSI color codes)
#--------------------------

BRED    = \033[1;31m
BGREEN  = \033[1;32m
BYELLOW = \033[1;33m
GREEN   = \033[1;35m
BCYAN   = \033[1;36m
RESET   = \033[0m

#-------
# Files
#-------

TEST = test
CODE = code
RUN = run

SOURCES = $(notdir $(wildcard $(SRCDIR)/*.cpp))

# Object files:
TEST_OBJ = $(BUILD)/$(TEST).o
CODE_OBJ = $(BUILD)/$(CODE).o
RUN_OBJ = $(BUILD)/$(RUN).o
OBJECTS = $(filter-out $(RUN_OBJ) $(TEST_OBJ) $(CODE_OBJ), $(SOURCES:%.cpp=$(BUILD)/%.o))

# Executable files
TEST_EXECUTABLE = $(BUILD)/$(TEST)
CODE_EXECUTABLE = $(BUILD)/$(CODE)
RUN_EXECUTABLE = $(BUILD)/$(RUN)

#---------------
# Build process
#---------------

default: $(TEST_EXECUTABLE) $(CODE_EXECUTABLE) $(RUN_EXECUTABLE)

# Link object files together
$(RUN_EXECUTABLE) : $(RUN_OBJ) $(OBJECTS)
	@printf "$(BYELLOW)Linking executable test $(BCYAN)$@$(RESET)\n"
	$(CC) $(LDFLAGS) $^ -o $@

# Link object files together
$(CODE_EXECUTABLE) : $(CODE_OBJ) $(OBJECTS)
	@printf "$(BYELLOW)Linking executable test $(BCYAN)$@$(RESET)\n"
	$(CC) $(LDFLAGS) $^ -o $@

# Link object files together
$(TEST_EXECUTABLE) : $(TEST_OBJ) $(OBJECTS)
	@printf "$(BYELLOW)Linking executable test $(BCYAN)$@$(RESET)\n"
	$(CC) $(LDFLAGS) $^ -o $@

# Build object files
$(BUILD)/%.o: $(SRCDIR)/%.cpp
$(BUILD)/%.o: $(SRCDIR)/%.cpp $(DEPDIR)/%.d Makefile | $(DEPDIR)
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p $(BUILD)
	$(CC) $(DEPFLAGS) $(CFLAGS) $< -c -o $@
	$(POSTCOMPILE)

# Create directory
$(DEPDIR):
	@mkdir -p $(DEPDIR)

# Dependency files
DEPFILES := $(SOURCES:%.cpp=$(DEPDIR)/%.d)

# Mention every .d file as a target so that Make won't fail if it doesn't exist
$(DEPFILES):

# Include all created dependencies in current makefile
include $(wildcard $(DEPFILES))

#-------------------
# Passing arguments
#-------------------

ifeq ($(CODE), $(firstword $(MAKECMDGOALS)))
  CODE_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(CODE_ARGS):;@:)
endif

ifeq ($(RUN), $(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif


#-----------------
# Run the program
#-----------------

# Run test:
$(TEST): $(TEST_EXECUTABLE)
	@mkdir -p res
	./$<

$(CODE): $(CODE_EXECUTABLE)
	@mkdir -p res
	./$< $(PROGDIR)/$(CODE_ARGS)

$(RUN): $(RUN_EXECUTABLE)
	@mkdir -p res
	./$< $(PROGDIR)/$(RUN_ARGS)


log:
	@cat hello.txt
#-------
# Other
#-------

clean:
	@printf "$(BYELLOW)Cleaning build and resource directories$(RESET)\n"
	rm -rf res
	rm -rf $(BUILD)
	rm -f programs/*.bcode

# List of non-file targets:
.PHONY: test clean default log