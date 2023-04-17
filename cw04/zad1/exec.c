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
        char *args[] = {"./tmp", NULL};
        execv(args[0], args);
    } else if (strcmp(argument, "mask") == 0)
    {
        set_mask();
        char *args[] = {"./tmp", NULL};
        execv(args[0], args);
    } else if (strcmp(argument, "pending") == 0)
    {
        set_mask();
        char *args[] = {"./pending", NULL};
        execv(args[0], args);
    } else
    {
        printf("Wrong argument. Try again.\n");
        return 2;
    }
    return 0;
}
