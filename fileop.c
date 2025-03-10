#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int fd;
    char data[] = "Sample file content\n";
    char buffer[1024];

    // Write to file
    fd = open("example.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open (write) failed");
        return 1;
    }
    write(fd, data, sizeof(data) - 1); // -1 to exclude null terminator
    close(fd);

    // Read from file
    fd = open("example.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("open (read) failed");
        return 1;
    }

    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("read failed");
        close(fd);
        return 1;
    }

    // write(STDOUT_FILENO, buffer, bytes_read);
    fprintf(stdout, buffer);
    close(fd);
    return 0;
}
