#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <time.h>
#include "common.h"

int queue;
int clients[MAX_CLIENTS][2];
int num_clients = 0;
int no_clients = 0;
bool had_client = false;
FILE *file;

typedef struct queue_message {
    long type;
    int id;
    char text[MESSAGE_BUFFER_SIZE];
    int pid;
    char _time[100];
} queue_message;

queue_message create_message(int id, long type) {
    queue_message message;
    message.id = id;
    message.type = type;
    return message;
}

void clean() {
    msgctl(queue, IPC_RMID, NULL);
    fclose(file);
}

void send_to_client(unsigned int client_id, queue_message *message) {
    if (client_id >= no_clients || clients[client_id][0] == -1) {
        fprintf(stderr, "Wrong client\n");
        return;
    }

    if (msgsnd(clients[client_id][0], message, MAX_MESSAGE_SIZE, 0) == -1) {
        perror("Couldn't send message to client\n");
    }
}

void handle_sigint(int signum) {
    if (num_clients == 0) {
        exit(0);
    }

    queue_message message;
    message.type = STOP;
    for (int i = 0; i < no_clients; ++i) {
        if (clients[i][0] != -1) {
            send_to_client(i, &message);
            kill(clients[i][1], SIGNAL);
        }
    }
}

int get_new_client_id() {
    if (no_clients < MAX_CLIENTS) {
        num_clients++;
        return no_clients++;
    }
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i][0] == -1) {
            num_clients++;
            return i;
        }
    }
    return -1;
}

void handle_init(queue_message *message) {
    key_t key;
    sscanf(message->text, "%d", &key);
    int pid = message->pid;

    int client_id = get_new_client_id();
    if (client_id != -1) {
        clients[client_id][0] = msgget(key, 0);
        if (clients[client_id][0] == -1) {
            perror("Can't open client queue\n");
        }

        clients[client_id][1] = pid;

        queue_message new_message = create_message(client_id, INIT);

        send_to_client(client_id, &new_message);

        had_client = true;
        printf("Client %d INIT successful\n", client_id);
    } else {
        fprintf(stderr, "Clients INIT error\n");
    }
}

void handle_stop(queue_message *message) {
    int client_id = message->id;
    clients[client_id][0] = -1;
    num_clients--;
    printf("Client: %d stops, clients left: %d\n", client_id, num_clients);
    if (num_clients == 0 && had_client) {
        exit(0);
    }
}

void handle_list() {
    printf("Active clients:\n");

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i][0] >= 0) {
            printf("Client %d\n", i);
        }
    }
}

void handle_one(queue_message *message) {
    int sender_id = message->id;
    int receiver_id = atoi(strtok(message->text, " "));
    printf("Server got message from %d to %d\n", sender_id, receiver_id);

    time_t _time = time(NULL);
    char *time_str = ctime(&_time);
    time_str[strlen(time_str) - 1] = '\0';

    queue_message new_message = create_message(sender_id, MESSAGE);
    sprintf(new_message.text, "%s", strtok(NULL, message->text));
    sprintf(new_message._time, "%s", time_str);

    send_to_client(receiver_id, &new_message);
    kill(clients[receiver_id][1], SIGNAL);
}

void handle_all(queue_message *message) {
    int sender_id = message->id;

    time_t _time = time(NULL);
    char *time_str = ctime(&_time);
    time_str[strlen(time_str) - 1] = '\0';

    queue_message new_message = create_message(sender_id, MESSAGE);
    sprintf(new_message.text, "%s", message->text);
    sprintf(new_message._time, "%s", time_str);

    for (int receiver_id = 0; receiver_id < MAX_CLIENTS; ++receiver_id) {
        if (clients[receiver_id][0] >= 0 && receiver_id != sender_id) {
            printf("Server got message from %d to %d\n", sender_id, receiver_id);

            send_to_client(receiver_id, &new_message);
            kill(clients[receiver_id][1], SIGNAL);
        }
    }
}

int main() {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clients[i][0] = -1;
    }

    atexit(clean);

    signal(SIGINT, handle_sigint);

    file = fopen("records", "a");
    if (file == NULL) {
        perror("Could not open the file\n");
        exit(1);
    }

    key_t key = ftok(getenv("HOME"), 8);
    if (key == -1) {
        perror("Can't generate public key\n");
        exit(1);
    }

    queue = msgget(key, IPC_CREAT | IPC_EXCL | 0600);
    if (queue == -1) {
        perror("Can't create queue\n");
        exit(1);
    }

    queue_message message;

    while (1) {

        if (msgrcv(queue, &message, MAX_MESSAGE_SIZE, -100, 0) == -1) {
            printf("Can't receive message\n");
            if (errno != EINTR) {
                perror("Can't receive message\n");
                exit(1);
            }

            continue;
        }

        time_t _time = time(NULL);
        char *time_str = ctime(&_time);
        time_str[strlen(time_str) - 1] = '\0';
        char client_id[10];
        sprintf(client_id, " %d ", message.id);

        char buff[MAX_MESSAGE_SIZE] = "Order details: ";
        strcat(buff, time_str);
        strcat(buff, client_id);

        switch (message.type) {

            case STOP: {
                strcat(buff, "STOP");
                strcat(buff, "\n");
                fwrite(buff, sizeof(buff), sizeof(char), file);
                handle_stop(&message);
                break;
            }
            case ALL: {
                handle_all(&message);
                strcat(buff, "2ALL ");
                strcat(buff, message.text);
                break;
            }
            case LIST: {
                handle_list();
                strcat(buff, "LIST");
                break;
            }
            case INIT: {
                handle_init(&message);
                strcat(buff, "INIT");
                break;
            }
            case ONE: {
                strcat(buff, "2ONE ");
                strcat(buff, message.text);
                handle_one(&message);
                break;
            }

            default: {
                fprintf(stderr, "Wrong message type\n");
                strcat(buff, "wrong type");
                break;
            }
        }

        strcat(buff, "\n");
        fwrite(buff, sizeof(buff), sizeof(char), file);
    }
    return 0;
}

