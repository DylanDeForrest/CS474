#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * Display a file on stdout.
 */
void cat(char *filename)
{
    char buf[4096];
    int count;

    int fd = open(filename, O_RDONLY);

    if (fd < 0) {
        perror("cat");
        return;
    }

    do {
        count = read(fd, buf, sizeof buf);
        write(1, buf, count);
    } while (count > 0);

    write(1, "\n", 1);

    close(fd);
}

/**
 * Main.
 */
int main(void)
{
    // Allowed functions: open, lseek, write, close

    // TODO: Open the file `seeker.txt` for writing

    // TODO: Write 60 `-` characters to the file

    // TODO: Seek to position 0
    // TODO: Write "interrupting"
    // TODO: cat("seeker.txt");

    // TODO: Seek to position 40
    // TODO: Write "cow"
    // TODO: cat("seeker.txt");

    // TODO: Seek to position 20
    // TODO: Write "MOOOOOOO!"
    // TODO: cat("seeker.txt");

    // TODO: Close the file
}