# Makefile for CPS222 Project 2

# Makefiles define rules for making files from other files.
# Rules have the a common form.
#    target:   dependency list
#        command to make target
# Note that the command is only run if the target is older than
# its dependencies.  For example, if a binary is older than the sources
# it is made from, it needs to be recompiled.

# Tabs are important in makefiles.
# There must be a tab after the ":".
# There must be a tab before each command.

# This rule makes the binary program from .o files.
# (.o files are made from .cc files using a rule further down.)
# The command uses some special variables:
#   $@  -- the name of the target
#   $^  -- the names of all dependencies (the .o files listed after ":")
project2: expression.o project2.o
	g++ -std=c++11 -o $@ $^

# This rule just gives an additional dependency.
# It doesn't have a command, because there's a command somewhere else (below).
expression.o:	expression.h

# This is a pattern matching rule.  Any file ending in .o can be made from
# a matching .cc file (note the "%"s).  The command uses a special variable:
#   $<  -- the first dependency (that is, the .cc file)
%.o:		%.cc
	g++ -std=c++11  -c $<

project2_instructor: expression_instructor.o project2.o
	g++ -std=c++11  -o $@ $^
