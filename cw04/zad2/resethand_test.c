#include <stdio.h>
#include <signal.h>
#define SIGNAL SIGUSR1

void handler(int signal)
{
    printf("Received signal: %d\n", signal);
    printf("Using set handler. Next time will be default.\n");
}

int main()
{
    struct sigaction sig_info;
    sig_info.sa_handler = handler;
    sig_info.sa_flags = SA_RESETHAND;

    if (sigaction(SIGNAL, &sig_info, NULL) < 0)
    {
        perror("Could not change signal handler.\n");
        return 1;
    }
    printf("Raising signal first time.\n");
    raise(SIGNAL);
    printf("Raising signal second time.\n");
    raise(SIGNAL);

    return 0;
}
