CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fopenmp
LFLAGS = 
LIBS = -lm
SOURCES = Q3.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = Q3
.PHONY: clean help

Q3 : Q3.c
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(EXE)

clean:
	rm -f $(OBJECTS) $(EXE) *~

cleanup:
	rm -f $(OBJECTS) *~

help:
	@echo "Valid targets:"
	@echo "  all:    generates all binary files"
	@echo "  clean:  removes .o and .exe files"
