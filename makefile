.SUFFIXES: .cpp .hpp

# Programs
SHELL 	= bash
CC     	= g++
LD	= ld
RM 	= rm
ECHO	= /bin/echo
CAT	= cat
PRINTF	= printf

######################################
# Project Name (generate executable with this name)
TARGET = ImageProcessor

# Project Paths
PROJECT_ROOT=./
SRCDIR = $(PROJECT_ROOT)/src
OBJDIR = $(PROJECT_ROOT)/objs
BINDIR = $(PROJECT_ROOT)/bin
INCLUDEDIR = $(PROJECT_ROOT)/include

# Compiler and Linker flags

CPPFLAGS=-I $(INCLUDEDIR)
GTKMM =`pkg-config gtkmm-3.0 --cflags --libs`
# -Wall -fno-strict-aliasing

# CPPFLAGS+=-I $(BOX2D_ROOT)/include -I $(GLUI_ROOT)/include
# CPPFLAGS_OPT+=-I $(BOX2D_ROOT)/include -I $(GLUI_ROOT)/include

# LDFLAGS+=-L $(BOX2D_ROOT)/lib -L $(GLUI_ROOT)/lib

######################################

NO_COLOR=\e[0m
OK_COLOR=\e[1;32m
ERR_COLOR=\e[1;31m
WARN_COLOR=\e[1;33m
MESG_COLOR=\e[1;34m
FILE_COLOR=\e[1;37m

OK_STRING="[OK]"
ERR_STRING="[ERRORS]"
WARN_STRING="[WARNINGS]"
OK_FMT="${OK_COLOR}%30s\n${NO_COLOR}"
ERR_FMT="${ERR_COLOR}%30s\n${NO_COLOR}"
WARN_FMT="${WARN_COLOR}%30s\n${NO_COLOR}"

######################################

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

.PHONY: all dirs dirs makesubmission exe clean 

all: dirs exe makesubmission

dirs:
	@$(ECHO) "Making directories"
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)


exe: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJS)
	
	@$(PRINTF) "$(MESG_COLOR)Building exe:$(NO_COLOR) $(FILE_COLOR) %23s$(NO_COLOR)" "$(notdir $@)"
	@$(CC) -o $@ $(OBJS) $(GTKMM) 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else $(PRINTF) $(OK_FMT) $(OK_STRING); \
	fi;
	@$(RM) -f temp.log temp.err

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(PRINTF) "$(MESG_COLOR)Compiling: $(NO_COLOR) $(FILE_COLOR) %25s$(NO_COLOR)" "$(notdir $<)"
	@$(CC) $(CPPFLAGS) -c $< -o $@ $(GTKMM) 2> temp.log || touch temp.err
	@if test -e temp.err; \
	then $(PRINTF) $(ERR_FMT) $(ERR_STRING) && $(CAT) temp.log; \
	elif test -s temp.log; \
	then $(PRINTF) $(WARN_FMT) $(WARN_STRING) && $(CAT) temp.log; \
	else $(PRINTF) "${OK_COLOR}%30s\n${NO_COLOR}" "[OK]"; \
	fi;
	@$(RM) -f temp.log temp.err

clean:
	@$(PRINTF) "Cleaning up...\n"
	@$(RM) -rf $(OBJDIR)/*.o
	@$(RM) -rf $(BINDIR)/*
	@$(RM) -rf $(OBJDIR)
	@$(RM) -rf $(BINDIR)
