#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "library.h"
#include <sys/times.h>
#include <time.h>
#include <ctype.h>

#define BUFFER_SIZE 50

struct tms start_counting;
struct tms stop_counting;
struct timespec start_time;
struct timespec end_time;

#ifdef DLL
#include <dlfcn.h>
#endif

int is_integer(char* string)
{
    size_t n = strlen(string);
    for (int i = 0; i < n; i++)
    {
        if (!isdigit(string[i]))
        {
            printf("Argument must be a number \n");
            return 0;
        }
    }
    return 1;
}

int if_missing_argument(const char *token)
{
    if (token == NULL)
    {
        printf("Missing argument \n");
        return 1;
    }
    return 0;
}

int if_too_many_arguments(char delimiter[])
{
    if (strtok(NULL, delimiter) != NULL)
    {
        printf("Wrong command \n");
        return 1;
    }
    return 0;
}

int if_array_exists(bool array_made)
{
    if (!array_made) {
        printf("Array does not exist \n");
        return 0;
    }
    return 1;
}

void counter_start()
{
    times(&start_counting);
    clock_gettime(CLOCK_REALTIME, &start_time);
}

void counter_stop()
{
    clock_gettime(CLOCK_REALTIME, &end_time);
    times(&stop_counting);

    long real_time = end_time.tv_nsec - start_time.tv_nsec;
    long user_time = (long) (stop_counting.tms_utime - start_counting.tms_utime) / CLOCKS_PER_SEC * 1e9;
    long system_time = (long) (stop_counting.tms_stime - start_counting.tms_stime) / CLOCKS_PER_SEC * 1e9;

    printf("Real time: %lu ns\nUser time: %lu ns\nSystem time: %lu ns\n\n", real_time, user_time, system_time);
}

int main()
{
#ifdef DLL
    void* lib_handle = dlopen("./liblibrary.so", RTLD_LAZY);
    memoryBlocks (*init)(int) = dlsym(lib_handle, "init");
    void (*count)(memoryBlocks*, const char*) = dlsym(lib_handle, "count");
    char* (*show)(memoryBlocks*, int) = dlsym(lib_handle, "show");
    void (*delete_index)(memoryBlocks*, int) = dlsym(lib_handle, "delete_index");
    void (*destroy)(memoryBlocks*) = dlsym(lib_handle, "destroy");
#endif

    bool array_made = false;
    char buffer[BUFFER_SIZE];
    char delimiter[] = " ";
    memoryBlocks memoryBlocks;
    printf("Enter a command: \n");
    while (fgets(buffer, BUFFER_SIZE, stdin))
    {
        if (buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
            char *token = strtok(buffer, delimiter);
            if (strcmp(token, "init") == 0)
            {
                token = strtok(NULL, delimiter);
                if (if_missing_argument(token) || if_too_many_arguments(delimiter) || !is_integer(token))
                {
                    continue;
                }

                if (array_made)
                {
                    printf("The array already exists \n");
                } else
                {
                    counter_start();
                    memoryBlocks = init(atoi(token));
                    counter_stop();

                    array_made = true;
                    printf("Made array \n");
                }
            } else if (strcmp(token, "count") == 0)
            {
                token = strtok(NULL, delimiter);
                if (if_missing_argument(token) || if_too_many_arguments(delimiter) || !if_array_exists(array_made))
                {
                    continue;
                }

                counter_start();
                count(&memoryBlocks, token);
                counter_stop();

            } else if (strcmp(token, "show") == 0)
            {
                token = strtok(NULL, delimiter);
                if (if_missing_argument(token) || if_too_many_arguments(delimiter) || !is_integer(token)
                    || !if_array_exists(array_made))
                {
                    continue;
                }

                counter_start();
                char* block_to_show = show(&memoryBlocks, atoi(token));
                counter_stop();

                printf("%s \n", block_to_show);
            } else if (strcmp(token, "delete") == 0)  // delete index index
            {
                token = strtok(NULL, delimiter);
                if (if_missing_argument(token) || if_too_many_arguments(delimiter) || !is_integer(token)
                    || !if_array_exists(array_made))
                {
                    continue;
                }

                counter_start();
                delete_index(&memoryBlocks, atoi(token));
                counter_stop();
            } else if (strcmp(token, "destroy") == 0)
            {

                if (if_too_many_arguments(delimiter) || !if_array_exists(array_made))
                {
                    continue;
                }

                array_made = false;

                counter_start();
                destroy(&memoryBlocks);
                counter_stop();

                printf("Destroyed array \n");
            } else
            {
                printf("Command does not exist \n");
            }
        }
    }
#ifdef DLL
    dlclose(lib_handle);
#endif

    return 0;
}
