#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>

int main(int argc, char* argv[])
{
    struct timespec start_time;
    struct timespec end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    if (argc != 3)
    {
        printf("Wrong number of arguments \n");
        return 0;
    }

    FILE *file_read = fopen(argv[1], "r");
    FILE *file_write = fopen(argv[2], "w");

    if (file_read == NULL) {
        printf("Could not open read file \n");
        return 0;
    }
    if (file_write == NULL) {
        printf("Could not open write file \n");
        return 0;
    }

    char* buff[1];
    bool bof = false;  // begin of file
    fseek(file_read, -1, SEEK_END);
    while (!bof)
    {
        if (ftell(file_read) == 0L)
        {
            bof = true;
        }

        fread(buff, 1, 1, file_read);
        fwrite(buff, 1, 1, file_write);
        fseek(file_read, -2, SEEK_CUR);
    }
    fclose(file_read);
    fclose(file_write);

    clock_gettime(CLOCK_REALTIME, &end_time);
    long time = end_time.tv_nsec - start_time.tv_nsec;
    printf("Execution time: %lu ns\n", time);

    return 0;
}

