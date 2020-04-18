#include "source.h"
#include <poll.h>
key_t client_KEY;
int client_ID;
int client_queue_ID;
int server_queue_ID;

int friend_queue_ID = -1;
//send section

void send_init(){
    
    ORDER_MESSAGE* order = malloc(sizeof(ORDER_MESSAGE));
    order->client_KEY = client_KEY;
    order->type = INIT_ORDER;
    msgsnd(server_queue_ID, order, sizeof(*order), 0);

    SERVER_MESSAGE* server = malloc(sizeof(SERVER_MESSAGE));

    msgrcv(client_queue_ID, server, sizeof(*server), SERVER_REGISTERED,0);

    client_ID = server->client_ID;
    printf("%s", server->server_message);
    free(order);
    free(server);

}

void send_list(){
    printf("[Me] sent LIST order\n");

    ORDER_MESSAGE* order = malloc(sizeof(ORDER_MESSAGE));
    order->client_ID = client_ID;
    order->type = LIST_ORDER;

    msgsnd(server_queue_ID, order, sizeof(*order), 0);

    SERVER_MESSAGE* server = malloc(sizeof(SERVER_MESSAGE));

    msgrcv(client_queue_ID, server, sizeof(*server), SERVER_LIST, 0);

    printf("%s", server->server_message);
    free(order);

}

void send_connect(int friend_ID){
    printf("[Me] sent CONNECT order to char: %d!\n", friend_ID);

    ORDER_MESSAGE* order = malloc(sizeof(ORDER_MESSAGE));
    order->client_ID = client_ID;
    order->chat_friend_ID = friend_ID;
    order->type = CONNECT_ORDER;

    msgsnd(server_queue_ID, order, sizeof(*order), 0);
    free(order);
}

void send_disconnect(){
    printf("[Me] sent DISCONNECT order!\n");

    ORDER_MESSAGE* order = malloc(sizeof(ORDER_MESSAGE));
    order->client_ID = client_ID;
    order->type = DISCONNECT_ORDER;

    msgsnd(server_queue_ID, order, sizeof(*order), 0);
    free(order);
    
    if(friend_queue_ID != -1){
        CHAT_MESSAGE* chat = malloc(sizeof(CHAT_MESSAGE));
        chat->type = CLIENT_DISCONNECT;

        msgsnd(friend_queue_ID, chat, sizeof(*chat), 0);
        friend_queue_ID = -1;
        free(chat);
    }
}

void send_stop(){
    printf("[Me] sent STOP order\n");

    ORDER_MESSAGE* order = malloc(sizeof(SERVER_MESSAGE));

    order->client_ID = client_ID;
    order->type = STOP_ORDER;

    msgsnd(server_queue_ID, order, sizeof(*order), 0);
    free(order);
    printf("Client exit!\n");
    exit(EXIT_SUCCESS);
}

void send_message(char* text){
    CHAT_MESSAGE* chat = malloc(sizeof(CHAT_MESSAGE));

    chat->type = CLIENT_MESSAGE;

    strcpy(chat->text, text);

    printf("###############\n");
    printf("[Me]: %s\n", chat->text);
    printf("###############\n");
    msgsnd(friend_queue_ID, chat, sizeof(*chat), 0);
}


//handle section


void handle_chat_init(SERVER_MESSAGE* server){
    printf("[Me] entered chat with [Friend %d] \n", server->client_ID);
    friend_queue_ID = msgget(server->chat_friend_KEY, 0666);
}

void handle_terminate(){
    printf("[Server] closing down , STOP order\n");
    send_stop();
}

void handle_disconnect(){
    printf("[Me] disconnected from chat\n");
    friend_queue_ID = -1;
    send_disconnect();
}

void handle_message(CHAT_MESSAGE* chat){

    printf("###############\n");
    printf("[Friend]: %s\n", chat->text);
    printf("###############\n");
}

void exit_signal(int number){
    send_stop();
}

void client_exit(){
    msgctl(client_queue_ID, IPC_RMID, NULL);
}

int empty_queue(int queue_ID){
    struct msqid_ds buffer;
    msgctl(queue_ID, IPC_STAT, &buffer);
    return buffer.msg_qnum == 0;
}

int main(int arg_num, char** args){
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };

    client_KEY = ftok(GENERATOR_PATH, getpid());
    
    client_queue_ID = msgget(client_KEY, 0666 | IPC_CREAT | IPC_EXCL);

    server_queue_ID = msgget((ftok(GENERATOR_PATH, PROJECT_ID)), 0666);

    signal(SIGINT, exit_signal);
    atexit(client_exit);
    char buffer[128];

    char text[MESSAGE_LIMIT];

    send_init();
    while(1){
        
        while(!empty_queue(client_queue_ID)){

            SERVER_MESSAGE* server = malloc(sizeof(SERVER_MESSAGE));
            CHAT_MESSAGE* chat = malloc(sizeof(CHAT_MESSAGE));

            
            if(msgrcv(client_queue_ID, server, sizeof(*server), SERVER_INIT_CHAT, IPC_NOWAIT) != -1){
                handle_chat_init(server);
            }
            if(msgrcv(client_queue_ID, server, sizeof(*server), SERVER_TERMINATE, IPC_NOWAIT) != -1){
                handle_terminate(server);
            }
            if(msgrcv(client_queue_ID, chat, sizeof(*chat), CLIENT_MESSAGE, IPC_NOWAIT) != -1){
                handle_message(chat);
            }
            if(msgrcv(client_queue_ID, chat, sizeof(*chat), CLIENT_DISCONNECT, IPC_NOWAIT)  != -1){
                handle_disconnect(chat);
            }

            free(server);
            free(chat);                    
        }

        if(poll(&mypoll, 1, 100))
        {
            scanf("%s", buffer);
            if(strcmp(buffer,"LIST")==0){
                send_list();
            }else if(strcmp(buffer, "CONNECT") == 0){
                int pchat_ID;
                scanf("%d", &pchat_ID);
                send_connect(pchat_ID);
            }else if(strcmp(buffer, "DISCONNECT") == 0){
                send_disconnect();
            }else if(strcmp(buffer, "SEND") == 0){
                scanf("%s", text);

                if(friend_queue_ID == -1){
                    printf("Not connect to chat!\n");
                }else{
                    send_message(text);
                }
            }else if(strcmp(buffer, "STOP") == 0){
                send_stop();
            }else{
                printf("Unknown command!\n");
            }
        }
    }
    return 0;
}