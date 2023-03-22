#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <ftw.h>

long long total_files_size = 0;

int fn(const char  *file_path, const struct stat *ptr, int flags)
{
    struct stat buffer_stat;
    stat(file_path, &buffer_stat);
    if (!S_ISDIR(buffer_stat.st_mode))
    {
        printf("%ld bytes - ", buffer_stat.st_size);
        printf("%s\n", file_path);
        total_files_size += buffer_stat.st_size;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Wrong number of arguments");
        return 0;
    }

    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    ftw(argv[1], fn, 10);

    printf("Total file size: %lld bytes\n", total_files_size);

    clock_gettime(CLOCK_REALTIME, &end_time);
    long time = end_time.tv_nsec - start_time.tv_nsec;
    printf("Execution time: %lu ns\n", time);

    return 0;
}
