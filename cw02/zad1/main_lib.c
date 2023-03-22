#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>


int main(int argc, char *argv[])
{
    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    FILE *old_file = fopen(argv[3], "r");
    FILE *new_file = fopen(argv[4], "w");

    if (argc != 5) {
        printf("Wrong number of arguments");
        return 0;
    }
    if (old_file == NULL)
    {
        printf("Could not open input file");
        return 0;
    }
    if (new_file == NULL)
    {
        printf("Could not open output file");
        return 0;
    }

    char* buffer[1];
    char *current_letter;
    while (fread(buffer, 1, 1, old_file) != 0)
    {
        if (strcmp(argv[1], (const char *) buffer) == 0)
        {
            current_letter = argv[2];
        } else
        {
            current_letter = (char *) buffer;
        }
        fwrite(current_letter, 1, 1, new_file);
    }
    fclose(old_file);
    fclose(new_file);

    clock_gettime(CLOCK_REALTIME, &end_time);
    long time = end_time.tv_nsec - start_time.tv_nsec;
    printf("Execution time: %lu ns\n", time);
    return 0;
}
