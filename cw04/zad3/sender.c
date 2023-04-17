#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <stdbool.h>

#define SIGNAL SIGUSR1

void handler(int signum)
{
    printf("Signal confirmed.\n");
}

int correct_number(char* string)
{
    for (int i = 0; i < strlen(string); ++i)
    {
        if (!isdigit(string[i]))
        {
            printf("Argument %s is not a number.\n", string);
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Expected at least 2 arguments. Got %d.\n", argc - 1);
        return 1;
    }
    if (!correct_number(argv[1]))
    {
        return 2;
    }
    signal(SIGNAL, handler);
    int pid = atoi(argv[1]);
    bool flag = false;
    for (int i = 2; i < argc; ++i)
    {
        if (correct_number(argv[i]))
        {
            if (strlen(argv[i]) == 1)
            {
                int mode = atoi(argv[i]);
                sigval_t value = {mode};
                sigqueue(pid, SIGNAL, value);

                sigset_t set;
                sigfillset(&set);
                sigdelset(&set, SIGNAL);
                sigsuspend(&set);
                if (mode == 5)
                {
                    flag = true;
                }
            } else
            {
                printf("Argument has wrong size. Expected 1, got %lu.\n", strlen(argv[i]));
            }
        }
        if (flag)
        {
            break;
        }
    }
    return 0;
}
