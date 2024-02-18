## explicitly add debugger support to each file compiled
CC = clang
CFLAGS = -std=c11 -Wall -g -pedantic
SRCDIR = ./src
BINDIR = ./bin

##
## TARGETS: below here we describe the target dependencies and rules
##

## build all targets
all : shell

## targets for each executable, based on the object files indicated
shell: main.o helper.o
	$(CC) $(CFLAGS) $(BINDIR)/main.o $(BINDIR)/helper.o -o $(BINDIR)/shell

main.o: $(SRCDIR)/main.c $(SRCDIR)/helper.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.c -o $(DIR)$(BINDIR)/main.o

helper.o: $(SRCDIR)/helper.c $(SRCDIR)/helper.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/helper.c -o $(DIR)$(BINDIR)/helper.o

## convenience target to remove the results of a build
clean:
	- rm -f $(BINDIR)/*



