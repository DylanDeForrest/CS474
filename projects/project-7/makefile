CC = gcc
CFLAGS = -Wall -Wextra -Werror
AR = ar
ARFLAGS = rcs

# Source files
LIB_SRCS = image.c block.c free.c inode.c mkfs.c
LIB_OBJS = $(LIB_SRCS:.c=.o)

TEST_SRCS = testfs.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_TARGET = testfs

# Default target
all: $(TEST_TARGET)

# Build test executable
$(TEST_TARGET): $(TEST_OBJS) $(LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(LIB_OBJS)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile testfs with CTEST_ENABLE
testfs.o: testfs.c
	$(CC) $(CFLAGS) -DCTEST_ENABLE -c $< -o $@

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Clean object files
clean:
	rm -f $(LIB_OBJS) $(TEST_OBJS)

# Clean everything including executables
pristine: clean
	rm -f $(TEST_TARGET)

.PHONY: all test clean pristine