#ifndef ZAD1_COMMON_H
#define ZAD1_COMMON_H

#define MESSAGE_BUFFER_SIZE 1024
#define MAX_MESSAGE_SIZE sizeof(queue_message) - sizeof(long)
#define SIGNAL SIGRTMIN
#define MAX_CLIENTS 20

#define STOP 1
#define LIST 2
#define ALL 3
#define ONE 4
#define INIT 5
#define MESSAGE 6

#endif //ZAD1_COMMON_H
