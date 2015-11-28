CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 
LFLAGS = 
LIBS = 
SOURCES = Q5.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = Q5
.PHONY: clean help

Q5 : Q5.c
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
