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

#-------
# Flags
#-------

CFLAGS = \
	-Wall \
	-Werror \
	-Wextra \
	-std=c++17 \
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

SOURCES = $(notdir $(wildcard $(SRCDIR)/*.cpp))

# Object files:
TEST_OBJ = $(BUILD)/$(TEST).o
OBJECTS = $(filter-out $(TEST_OBJ), $(SOURCES:%.cpp=$(BUILD)/%.o))

# Executable files
TEST_EXECUTABLE = $(BUILD)/$(TEST)

#---------------
# Build process
#---------------

default: $(TEST_EXECUTABLE)

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

#-----------------
# Run the program
#-----------------

# Run test:
$(TEST): $(TEST_EXECUTABLE)
	@mkdir -p res
	./$(TEST_EXECUTABLE)

#-------
# Other
#-------

clean:
	@printf "$(BYELLOW)Cleaning build and resource directories$(RESET)\n"
	rm -rf res
	rm -rf $(BUILD)

# List of non-file targets:
.PHONY: test clean default