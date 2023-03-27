#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {
    long number_of_processes;
    char *text_part;
    if (argc != 2)
    {
        printf("Wrong number of arguments\n");
        return 0;
    }
    number_of_processes = strtol(argv[1], &text_part, 10);
    if (strcmp(text_part, "") != 0)
    {
        printf("Argument must be a number\n");
        return 0;
    }

    pid_t parent_pid = getpid();
    for (int i = 0; i < number_of_processes; ++i) {
        if (getpid() == parent_pid)
        {
            __pid_t pid = fork();

            if (pid == 0)
            {
                printf("parent pid: %d, child pid: %d\n", getppid(), getpid());
                exit(0);
            }
        }
    }
    for (int i = 0; i < number_of_processes; ++i)
    {
        int status;
        wait(&status);
    }

    printf("Number of child processes: %ld\n", number_of_processes);

    return 0;
}
