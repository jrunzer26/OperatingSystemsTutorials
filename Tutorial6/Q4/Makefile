CC = gcc
CFLAGS = -Wall -Wextra -std=c99 
LFLAGS = 
LIBS = -lm -lpthread
SOURCES = Q4.c
OBJECTS = $(subst .c,.o,$(SOURCES))
EXE = Q4
.PHONY: clean help

Q4 : Q4.c
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