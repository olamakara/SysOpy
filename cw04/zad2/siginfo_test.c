#include <stdio.h>
#include <signal.h>
#define SIGNAL SIGINT

void handler(int signal, siginfo_t *info, void *context)
{
    printf("Received signal: %d\n", signal);
    printf("Signal number: %d\n", info->si_signo);
    printf("PID number: %d\n", info->si_pid);
    printf("Signal code: %d\n", info->si_code);
    printf("UID number: %d\n", info->si_uid);
    printf("Exit value/signal for process termination: %d\n", info->si_status);
}

int main()
{
    struct sigaction sig_info;
    sig_info.sa_sigaction = handler;
    sig_info.sa_flags = SA_SIGINFO;

    if (sigaction(SIGNAL, &sig_info, NULL) < 0)
    {
        perror("Could not change signal handler.\n");
        return 1;
    }

    raise(SIGNAL);

    return 0;
}
