#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define SIGNAL SIGCHLD

void handler(int signal)
{
    printf("Will not terminate stopped child processes.\n");
}

int main()
{
    int parent_pid = getpid();
    printf("Parent PID: %d\n", parent_pid);
    struct sigaction sig_info;
    sig_info.sa_handler = handler;
    sig_info.sa_flags = SA_NOCLDSTOP;

    if (sigaction(SIGNAL, &sig_info, NULL) < 0)
    {
        perror("Could not change signal handler.\n");
        return 1;
    }
    int pid_stop = 0;
    if (fork() == 0)
    {
        pid_stop = getpid();
        printf("PID of a child process which is not going to be terminated: %d\n", pid_stop);
        raise(SIGSTOP);
    } else
    {
        sleep(1);
    }
    if (getpid() == parent_pid)
    {
        raise(SIGCHLD);
        printf("Raised SIGCHLD signal.\n");
        kill(pid_stop, SIGCONT);
        printf("Continued stopped child process.\n");
    }

    printf("PID after SIGCHLD: %d\n", getpid());
    sleep(1);

    return 0;
}
