CC = gcc
CFLAGS = -Wall -Wextra -std=c11
EXE = a6
SRCS = a6.c
HEADERS = a6.h
all: $(EXE)

$(EXE): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(EXE) $(SRCS)

clean:
	rm -f $(EXE)

.PHONY: all clean
