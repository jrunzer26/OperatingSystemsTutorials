CC = gcc
CFLAGS = -Wall -Wextra -std=c99 
LFLAGS = 
LIBS = -lm -lpthread
SOURCES = Q2.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = Q2
.PHONY: clean help

Q2 : Q2.c
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
