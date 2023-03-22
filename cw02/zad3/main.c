#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>

int main()
{
    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    DIR *directory = opendir(".");
    if (directory == NULL)
    {
        printf("Unable to read directory");
        return 1;
    }

    struct dirent *read;
    struct stat buffer_stat;
    long long total_files_size = 0;
    while ((read = readdir(directory)))
    {
        stat(read->d_name, &buffer_stat);
        if (!S_ISDIR(buffer_stat.st_mode))
        {
            printf("%s - ", read->d_name);
            printf("%ld bytes\n", buffer_stat.st_size);
            total_files_size += buffer_stat.st_size;
        }
    }
    closedir(directory);
    printf("Total file size: %lld bytes\n", total_files_size);

    clock_gettime(CLOCK_REALTIME, &end_time);
    long time = end_time.tv_nsec - start_time.tv_nsec;
    printf("Execution time: %lu ns\n", time);

    return 0;
}
