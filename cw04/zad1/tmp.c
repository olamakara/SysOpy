#include <signal.h>

#define SIGNAL SIGPIPE

int main()
{
    raise(SIGNAL);
    return 0;
}