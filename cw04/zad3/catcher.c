#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define SIGNAL SIGUSR1
int counter = 0;
bool flag_4 = false;
int pid;

void handler(int signum, siginfo_t *siginfo, void *context)
{
    flag_4 = false;
    printf("Signal received.\n");
    kill(siginfo->si_pid, SIGNAL);
    counter++;

    int mode = siginfo->si_status;
    if (mode == 1)
    {
        for (int i = 1; i < 101; ++i) {
            printf("%d\n", i);
        }
    } else if (mode == 2)
    {
        time_t t;
        time(&t);
        printf("Current local time and date: %s\n", ctime(&t));
    } else if (mode == 3)
    {
        printf("Operation mode has been changed %d times.\n", counter);
    } else if (mode == 4) {
        flag_4 = true;
    } else if (mode == 5)
    {
        kill(pid, SIGALRM);
    } else
    {
        printf("Expected operation number between 1-5.\n");
        counter--;
    }
}

int main()
{
    pid = getpid();
    printf("Catcher PID: %d\n", pid);
    struct sigaction sig_info;
    sig_info.sa_sigaction = handler;
    sig_info.sa_flags = SA_SIGINFO;

    if (sigaction(SIGNAL, &sig_info, NULL) < 0)
    {
        perror("Could not change signal handler.\n");
        return 1;
    }

    while (1)
    {
        printf("Waiting for SIGUSR1 signal...\n");
        while (flag_4) {
            time_t t;
            time(&t);
            printf("Current local time and date: %s\n", ctime(&t));
            sleep(1);
        }
        pause();
    }

    return 0;
}
