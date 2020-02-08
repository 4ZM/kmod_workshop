#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const * argv[] ) {

    if (argc != 2)
        goto error;

    // Can the file be opened and read? Does it exist?
    if (access(argv[1], R_OK) < 0)   // syscall: access
        goto error;

    int fd = open(argv[1], O_RDONLY); // syscall: open
    if (fd == 0)
        goto error;

    // Read the file and write it to stdout (one chunk at a time)
    char buff[1024];
    int nread;
    do {
        nread = read(fd, buff, 1024); // syscall: read
        write(1, buff, nread); // syscall: write
    } while(nread > 0);

    close(fd); // syscall: close

    return 0;

error:
    return -1;
}
