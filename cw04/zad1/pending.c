#include <signal.h>
#include <stdio.h>

#define SIGNAL SIGPIPE

int main()
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
    return 0;
}