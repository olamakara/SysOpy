#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "common.h"

int server_queue, client_queue;
int id;
int queue;
int clients[MAX_CLIENTS][2];
int num_clients = 0;
int no_clients = 0;

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

int empty(int queue_id)
{
    struct msqid_ds buff;
    if (msgctl(queue_id, IPC_STAT, &buff) == -1) {
        perror("Couldn't get queue information\n");
        exit(1);
    }
    return buff.msg_qnum == 0;
}

void get_command(char *text, char *command, char *text_left) {
    char copy[256];
    strcpy(copy, text);
    char *line_tmp = copy;
    char *cmd = strtok_r(copy, " \n", &line_tmp);
    char *txt = strtok_r(NULL, "\n", &line_tmp);
    if (txt == NULL) {
        text_left[0] = '\0';
    } else {
        strcpy(text_left, txt);
    }
    if (cmd == NULL) {
        command[0] = '\0';
    } else {
        strcpy(command, cmd);
    }
}

void send_to_server(queue_message *message) {
    if (msgsnd(server_queue, message, MAX_MESSAGE_SIZE, 0)) {
        perror("Couldn't send message to server\n");
    }
}

void clean() {
    queue_message message = create_message(id, STOP);
    send_to_server(&message);
    msgctl(client_queue, IPC_RMID, NULL);
}

void handle_sigint(int signum) {
    exit(0);
}

void handle_stop() {
    kill(getpid(), SIGINT);
}

void handle_list() {
    queue_message message = create_message(id, LIST);
    send_to_server(&message);
}

void handle_2_one(char* message) {
    queue_message new_message = create_message(id, ONE);
    sprintf(new_message.text, "%s", message);
    send_to_server(&new_message);
    char* receiver_id = strtok(message, " ");
    message = strtok(NULL, " ");
    printf("To %s: %s\n", receiver_id, message);
}

void handle_2_all(char* message) {
    queue_message new_message = create_message(id, ALL);
    sprintf(new_message.text, "%s", message);
    send_to_server(&new_message);
    printf("To all: %s\n", message);
}

void init() {
    key_t key = ftok(getenv("HOME"), 8);
    if (key == -1) {
        perror("Can't generate public key\n");
        exit(1);
    }

    server_queue = msgget(key, 0);
    if (server_queue == -1) {
        perror("Can't open server queue\n");
        exit(1);
    }

    key_t client_key = ftok(getenv("HOME"), getpid());
    if (client_key == -1) {
        perror("Can't generate client key\n");
        exit(1);
    }

    client_queue = msgget(client_key, IPC_CREAT | IPC_EXCL | 0600);
    if (client_queue == -1) {
        perror("Can't create client queue\n");
        exit(1);
    }

    queue_message message;
    message.type = INIT;
    sprintf(message.text, "%d", client_key);
    message.pid = getpid();

    if (msgsnd(server_queue, &message, MAX_MESSAGE_SIZE, 0) == -1) {
        perror("Could not send INIT to server\n");
        exit(1);
    }

    if (msgrcv(client_queue, &message, MAX_MESSAGE_SIZE, -100, 0) == -1) {
        perror("Did not receive INIT respond from server\n");
        exit(1);
    }

    id = message.id;
    printf("INIT successful, client id: %d\n", id);
}

void send_command() {
    char text[MESSAGE_BUFFER_SIZE], command[MESSAGE_BUFFER_SIZE], text_left[MESSAGE_BUFFER_SIZE];
    fgets(text, MESSAGE_BUFFER_SIZE, stdin);
    get_command(text, command, text_left);

    if (strcmp("STOP", command) == 0) {
        handle_stop();
    } else if (strcmp("LIST", command) == 0) {
        handle_list();
    } else if (strcmp("2ALL", command) == 0) {
        handle_2_all(text_left);
    } else if (strcmp("2ONE", command) == 0) {
        handle_2_one(text_left);
    }
}

void handle_message(queue_message *message) {
    printf("%s\n", message->_time);
    printf("From %d: %s\n", message->id, message->text);
}

void catch_command() {
    signal(SIGINT, handle_sigint);

    while (!empty(client_queue)) {
        queue_message message;

        if (msgrcv(client_queue, &message, MAX_MESSAGE_SIZE, -100, IPC_NOWAIT) != -1) {

            switch (message.type) {
                case STOP:
                    printf("exiting\n");
                    exit(0);
                case MESSAGE:
                    printf("Got message\n");
                    handle_message(&message);
                    break;
                default:
                    break;
            }
        }
    }
}

int main() {

    atexit(clean);

    signal(SIGNAL, catch_command);
    signal(SIGINT, handle_sigint);

    init();

    while (1) {
        send_command();
        catch_command();
    }

    return 0;
}
