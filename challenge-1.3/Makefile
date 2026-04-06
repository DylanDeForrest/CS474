CC=gcc
CCOPTS=-Wall -Wextra
LIBS=
MAKESKELETON=../../../../bin/makeskeleton

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
TARGET=rrsim
ZIPDIR=challenge-1.3
ZIPFILE=$(ZIPDIR).zip

.PHONY: all clean pristine zip

all: rrsim

$(TARGET): rrsim.o queue.o
	$(CC) $(CCOPTS) -o $@ $^

%.o: %.c
	$(CC) $(CCOPTS) -c $<

rrsim.o: rrsim.c queue.h

queue.o: queue.c queue.h
    
rrsim_skel.c: rrsim.c
	$(MAKESKELETON) $< $@
	
clean:
	rm -f $(OBJS)
	rm -f rrsim_skel.c

pristine: clean
	rm -f $(TARGET) $(ZIPFILE)

zip: rrsim_skel.c
	rm -rf $(ZIPDIR)
	mkdir -p $(ZIPDIR)
	cp Makefile queue.c queue.h $(ZIPDIR)
	cp rrsim_skel.c $(ZIPDIR)/rrsim.c
	zip -r $(ZIPFILE) $(ZIPDIR)
	rm -rf $(ZIPDIR)
