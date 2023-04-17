#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#define SIGNAL SIGPIPE

void ignore(int signum)
{
    signal(signum, SIG_IGN);
    printf("Set ignore.\n");
}

void handler(int signum)
{
    printf("Handler received.\n");
}

void set_handler(int signum)
{
    signal(signum, handler);
    printf("Set handler.\n");
}

void mask(int signum)
{
    printf("Signal number: %d\n", signum);
}

void set_mask()
{
    signal(SIGNAL, mask);
    raise(SIGNAL);
    sigset_t new_mask;
    sigset_t old_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGNAL);
    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0)
    {
        perror("Could not set new mask.\n");
    } else
    {
        printf("Set new mask.\n");
    }
}

int main(int argc, char* argv[])
{
    printf("parent pid: %d\n", getpid());
    if (argc != 2)
    {
        printf("Wrong number of arguments. Expected one.\n");
        return 1;
    }
    char *argument = argv[1];
    if (strcmp(argument, "ignore") == 0)
    {
        signal(SIGNAL, ignore);
        raise(SIGNAL);
        if (fork() == 0)
        {
            raise(SIGNAL);
        }
    } else if (strcmp(argument, "handler") == 0)
    {
        signal(SIGNAL, set_handler);
        raise(SIGNAL);
        if (fork() == 0)
        {
            raise(SIGNAL);
        }
    } else if (strcmp(argument, "mask") == 0)
    {
        set_mask();
        if (fork() == 0)
        {
            raise(SIGNAL);
        }
    } else if (strcmp(argument, "pending") == 0)
    {
        set_mask();
        raise(SIGNAL);
        if (fork() == 0)
        {
            sigset_t set;
            if (sigpending(&set) != 0)
            {
                perror("Could not get pending signals.\n");
            } else if (sigismember(&set, SIGNAL) == 1)
            {
                printf("Signal is pending.\n");
            } else
            {
                printf("Signal is not pending.\n");
            }
        }
    } else
    {
        printf("Wrong argument. Try again.\n");
        return 2;
    }
    return 0;
}
