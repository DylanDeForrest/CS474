CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11

TARGET  = myalloc
SRC     = myalloc.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)