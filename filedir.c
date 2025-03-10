#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

int main()
{
    DIR *directory;
    struct dirent *entry;

    // Open the current directory
    directory = opendir(".");
    if (!directory)
    {
        perror("Failed to open directory");
        return 1;
    }

    printf("Contents of the current directory:\n");

    // Read and list all entries in the directory
    while ((entry = readdir(directory)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            printf("Directory: %s\n", entry->d_name);
        }
        else
        {
            printf("File: %s\n", entry->d_name);
        }
    }

    // Close the directory
    closedir(directory);
    return 0;
}
