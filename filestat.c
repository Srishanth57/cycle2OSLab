#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

int main()
{
    const char *filename = "example.txt"; // Specify the filename directly
    struct stat file_info;

    if (stat(filename, &file_info) == -1)
    {
        perror("stat failed");
        return 1;
    }

    printf("File Metadata for: %s\n", filename);
    printf("Size: %ld bytes\n", file_info.st_size);
    printf("Permissions: %o\n", file_info.st_mode & 0777);
    printf("Last Modified: %s", ctime(&file_info.st_mtime));

    return 0;
}
