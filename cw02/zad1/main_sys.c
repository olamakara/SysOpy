#include <time.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    int old_file = open(argv[3], O_RDONLY);
    int new_file = open(argv[4], O_WRONLY);

    if (argc != 5) {
        printf("Wrong number of arguments");
        return 0;
    }
    if (old_file == -1)
    {
        printf("Could not open input file");
        return 0;
    }
    if (new_file == -1)
    {
        printf("Could not open output file");
        return 0;
    }

    char* buffer[1];
    char *current_letter;
    while (read(old_file, buffer, 1) != 0)
    {
        if (strcmp(argv[1], (const char *) buffer) == 0)
        {
            current_letter = argv[2];
        } else
        {
            current_letter = (char *) buffer;
        }
        write(new_file, current_letter, 1);
    }
    close(old_file);
    close(new_file);

    clock_gettime(CLOCK_REALTIME, &end_time);
    long time = end_time.tv_nsec - start_time.tv_nsec;
    printf("Execution time: %lu ns\n", time);
    return 0;
}
