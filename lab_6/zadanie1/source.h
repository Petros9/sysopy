#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define MESSAGE_LIMIT 128
#define CLIENTS_LIMIT 8
#define PROJECT_ID (888041)
#define GENERATOR_PATH (getenv("HOME"))
#define SERVER_MESSAGE_TYPE_PRIORITY (-6)
#define SERVER_MESSAGE_LIMIT 128
// Orders: Client -> Server section

#define STOP_ORDER 1
#define DISCONNECT_ORDER 2
#define LIST_ORDER 3
#define CONNECT_ORDER 4
#define INIT_ORDER 5
// Inter Chat section

#define SERVER_INIT_CHAT 1
#define SERVER_TERMINATE 2
#define SERVER_REGISTERED 3
#define CLIENT_MESSAGE 4
#define CLIENT_DISCONNECT 5
#define SERVER_LIST 6

// structures

struct CHAT_MESSAGE{
    long type;
    char text[MESSAGE_LIMIT];
}typedef CHAT_MESSAGE;


struct ORDER_MESSAGE{
    long type;
    int client_ID;
    key_t client_KEY;
    int chat_friend_ID;
}typedef ORDER_MESSAGE;

struct SERVER_MESSAGE{
    long type;
    int client_ID;
    key_t chat_friend_KEY;
    char server_message[SERVER_MESSAGE_LIMIT];
}typedef SERVER_MESSAGE;

struct CLIENT{
    int client_ID;
    int queue_ID;
    int online;
    key_t client_KEY;
}typedef CLIENT;
