# Filesystem Project

## Building

Command line:

* `make` to build. produces a executable called `testfs`
* `make test` to run the tests
* `make clean` to clean up build products except the executable.
* `make pristine` to clean up everything

## Files

* `image.c`: opening and closing the image file
* `block.c`: reading and writing blocks also alloc
* `free.c`: finding and setting free bits in a bitmap
* `inode.c`: all the inode stuff, in-core inodes, read/write, iget iput ialloc
* `mkfs.c`: makes the filesystem
* `pack.c`: helper functions for reading and writing multibyte values
* `testfs.c`: all the tests

## Data

The main data is the disk image file. its broken into 4096 byte blocks.

block 0 is unused, block 1 is the free inode bitmap, block 2 is the free block bitmap, blocks 3-6 hold inodes, and block 7+ is data blocks.

inodes are stored as 64 byte records on disk. in memory we use a struct inode which has all the same fields plus ref_count and inode_num for tracking.

theres also a global array of 64 in-core inodes called incore that keeps track of whats currently open.

## Functions

* `image_open()`: opens the image file
* `image_close()`: closes it
* `bread()`: reads a block from disk
* `bwrite()`: writes a block to disk
* `alloc()`: finds a free block and marks it used
* `find_free()`: finds first free bit in bitmap
* `set_free()`: sets or clears a bit in the bitmap
* `incore_find_free()`: finds a free slot in the incore array
* `incore_find()`: finds an incore inode by number
* `incore_free_all()`: resets all ref counts to 0
* `read_inode()`: reads inode data from disk into a struct
* `write_inode()`: writes struct data back to disk
* `iget()`: gets an in-core inode for a given number
* `iput()`: releases an in-core inode, writes to disk if ref hits 0
* `ialloc()`: allocates a new inode and returns in-core version
* `mkfs()`: initializes the filesystem

## Notes

* resources used: class slides and the project spec
* i totally forgot to add a readme in the previous projects cause im a bad student that turns everythign in late 