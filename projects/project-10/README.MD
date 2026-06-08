# Filesystem Project
## Building
* `make` to build, produces executable called `testfs`
* `make test` to run tests
* `make clean` to clean up build products except the executable
* `make pristine` to clean up everything
## Files
* `image.c`: opening and closing the image file
* `block.c`: reading and writing blocks also alloc
* `free.c`: finding and setting free bits in a bitmap
* `inode.c`: all the inode stuff, in-core inodes, read/write, iget iput ialloc
* `mkfs.c`: makes the filesystem, now also creates root directory
* `pack.c`: helper functions for reading and writing multibyte values
* `dir.c`: directory open, read, close, plus namei and directory_make
* `ls.c`: ls function that prints directory entries, takes a inode number now
* `testfs.c`: all the tests
## Data
Same as before, disk image split into 4096 byte blocks.
block 0 unused, block 1 free inode bitmap, block 2 free block bitmap, blocks 3-6 hold inodes, block 7+ is data.
New this week: root directory lives at inode 0 and gets created by mkfs(). it has two hardcoded entries, . and .. which both point to inode 0 since theres no parent above root.
## Functions
same ones as before plus:
* `directory_open()`: takes an inode number, returns a directory struct
* `directory_get()`: reads the next entry from an open directory
* `directory_close()`: closes it and frees memory
* `ls()`: takes a inode number now, opens that dir and prints all the entries
* `namei()`: takes a path string and returns the inode for it, if path is just / returns root, otherwise looks through root for the name. returns NULL if it cant find it
* `directory_make()`: makes a new directory inside root, sets up . and .. entries and adds it to the parents directory block
## Notes
* talked with Nick and Sean about how namei should work
* resources used: class slides and project spec