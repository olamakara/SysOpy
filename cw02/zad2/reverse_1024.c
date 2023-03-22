#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

char* reverse_buff(char buff[], unsigned long n)
{
    char tmp;
    for (int i = 0; i < n / 2; i++)
    {
        tmp = buff[i];
        buff[i] = buff[n - i - 1];
        buff[n - i - 1] = tmp;
    }
    return buff;
}

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

    char buff[1024];
    bool bof = false;  // begin of file
    unsigned long bytes;
    fseek(file_read, -1024, SEEK_END);
    while (!bof)
    {
        if (ftell(file_read) == 0L)
        {
            bof = true;
        }

        bytes = fread(buff, 1, 1024, file_read);
        fwrite(reverse_buff(buff, bytes), 1, bytes, file_write);
        fseek(file_read, -2048, SEEK_CUR);
    }
    fclose(file_read);
    fclose(file_write);

    clock_gettime(CLOCK_REALTIME, &end_time);
    long time = end_time.tv_nsec - start_time.tv_nsec;
    printf("Execution time: %lu ns\n", time);

    return 0;
}