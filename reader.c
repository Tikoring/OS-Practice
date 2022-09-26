#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

int main (void) {
    int fd, n;
    char buf[256];
    DIR *dp;
    struct dirent *dent;
    long loc;
    struct stat statBuf;

    char *cwd = getcwd (NULL, BUFSIZ);
    if ((dp = opendir (cwd)) == NULL) {
        perror ("opendir: source");
        exit (1);
    }

    while (readdir (dp));
    loc = telldir (dp);
    rewinddir (dp);

    while (dent = readdir (dp)) {
        if (strcmp (dent->d_name, "data.txt") == 0) {
            break;
        }
        if (telldir (dp) == loc) {
            rewinddir (dp);
        }
    }

    closedir (dp);

    stat ("data.txt", &statBuf);
    if (((unsigned int)statBuf.st_mode & S_IFMT) != S_IFREG) {
        perror ("data.txt is not regular file");
        exit (1);
    }
    if (((unsigned int)statBuf.st_mode) & S_IRWXG | S_IRWXO) {
        perror ("data.txt must be protected");
        exit (1);
    }

    fd = open ("data.txt", O_RDONLY);
    if (fd == -1) {
        perror ("Open data.txt");
        exit (1);
    }
    
    while (1) {
        n = read(fd, buf, 255);
        buf[n] = '\0';

        if (n == -1) {
            perror ("Read error");
        } else if (n == 0) continue;

        write (1, "Recv>> ", 7);
        write (1, buf, n);

        if (n == 1 && buf[0] == 'q') {
            write (1, "Terminate\n", 10);
            break;
        }
    }

    close (fd);

    return 0;
}