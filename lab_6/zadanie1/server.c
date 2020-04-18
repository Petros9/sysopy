#include "source.h"


CLIENT* clients_list[CLIENTS_LIMIT];
int server_queue_ID = -1;
int clients_online = 0;
int termination = 0;
int current = 0;

void handle_init(ORDER_MESSAGE* order){
  int position = -1;
  for (int i = 0; i < CLIENTS_LIMIT; i++) {
    position = (current + i) % CLIENTS_LIMIT;
    if (!clients_list[position]) {
      break;
    }
  }
    if(position == -1){
        printf("Server cannot add the client, too many users!\n");
    }else{
        CLIENT* client = malloc(sizeof(CLIENT));
        client->client_ID = position;
        client->client_KEY = order->client_KEY;
        client->queue_ID = msgget(order->client_KEY, 0666);
        client->online = 1;

        clients_list[position] = client;


        SERVER_MESSAGE* server = malloc(sizeof(SERVER_MESSAGE));
        server->client_ID = position;
        server->type = SERVER_REGISTERED;
        sprintf(server->server_message,"You are registered with ID: %d, key: %d\n", client->client_ID, client->client_KEY);

        msgsnd(client->queue_ID, server, sizeof(*server), 0);
        clients_online++;
        printf("Server got INIT order from client: %d\n", client->client_ID);
        free(server); 
    }
}

void handle_list(ORDER_MESSAGE* order){

    SERVER_MESSAGE* server = malloc(sizeof(SERVER_MESSAGE));
    server->type = SERVER_LIST;
    printf("Server got LIST order\n");
    strcpy(server->server_message, "\nList of available friends: \n");
    for(int i = 0; i< CLIENTS_LIMIT; i++){
        if(i == order->client_ID){
            char answer[40];
            sprintf(answer,"\t[Me] -> id: %d, key: %d\n", clients_list[i]->client_ID, clients_list[i]->client_KEY);
            strcat(server->server_message, answer);
        }else if(clients_list[i] && clients_list[i]->online){
            char answer[40];
            sprintf(answer,"\t[Friend] -> id: %d, key: %d\n", clients_list[i]->client_ID, clients_list[i]->client_KEY);
            strcat(server->server_message, answer);
        }
    }
    msgsnd(clients_list[order->client_ID]->queue_ID, server, sizeof(*server), 0);
}

void handle_connect(ORDER_MESSAGE* order){
    int client_ID = order->client_ID;
    int chat_friend_ID = order->chat_friend_ID;


    if(!clients_list[chat_friend_ID] || chat_friend_ID > CLIENTS_LIMIT || chat_friend_ID <0){
        printf("Server got wrong chat number!\n");
        return;
    }

    clients_list[client_ID]->online = 0;
    clients_list[chat_friend_ID]->online = 0;

    SERVER_MESSAGE* server1 = malloc(sizeof(SERVER_MESSAGE));
    SERVER_MESSAGE* server2 = malloc(sizeof(SERVER_MESSAGE));
    
    server1->type = SERVER_INIT_CHAT;
    server2->type = SERVER_INIT_CHAT;

    server1->chat_friend_KEY = clients_list[chat_friend_ID]->client_KEY;
    server2->chat_friend_KEY = clients_list[client_ID]->client_KEY;

    server1->client_ID = client_ID;
    server2->client_ID = chat_friend_ID;

    msgsnd(clients_list[client_ID]->queue_ID, server1, sizeof(*server1), 0);
    msgsnd(clients_list[chat_friend_ID]->queue_ID, server2, sizeof(*server2), 0);


    printf("Server got CONNECT order from client %d, with client %d\n", client_ID, chat_friend_ID);
}

void handle_disconnect(ORDER_MESSAGE* order){
    printf("server got DISCONNECT order from client %d -> now the client is available\n", order->client_ID);
    clients_list[order->client_ID]->online = 1;
}


void handle_stop(ORDER_MESSAGE* order){
    clients_list[order->client_ID] = NULL;
    free(clients_list[order->client_ID]);

    clients_online--;
    printf("Server got STOP order from client: %d\n", order->client_ID);

    if(termination == 0 && clients_online == 0){
        printf("Server unavailable\n");
        exit(EXIT_SUCCESS);
    }

}


void handle_message(){
    ORDER_MESSAGE* order = malloc(sizeof(ORDER_MESSAGE));
    msgrcv(server_queue_ID, order, sizeof(*order), 0, 0);
    int type = order->type;

    if(type == INIT_ORDER){
        handle_init(order);
    }else if(type == LIST_ORDER){
        handle_list(order);
    }else if(type == CONNECT_ORDER){
        handle_connect(order);
    }else if(type == DISCONNECT_ORDER){
        handle_disconnect(order);
    }else if(type == STOP_ORDER){
        handle_stop(order);
    }
    free(order);
}

void exit_signal(int signal){
    SERVER_MESSAGE* server = malloc(sizeof(SERVER_MESSAGE));
    server->type = SERVER_TERMINATE;
    printf("\tclients online: %d\n", clients_online);
    if(clients_online == 0){
        printf("Server unavailable\n");
        exit(EXIT_SUCCESS);
    }else{
        for(int i = 0; i<CLIENTS_LIMIT; i++){
            if (clients_list[i]){
                msgsnd(clients_list[i]->queue_ID, server, sizeof(*server),0);
            }
        }
        printf("Server unavailable\n");
        exit(EXIT_SUCCESS);
    }
}

void server_exit(){
    msgctl(server_queue_ID, IPC_RMID, NULL);
}

int main(int arg_num, char** args){

    server_queue_ID = msgget((ftok(GENERATOR_PATH, PROJECT_ID)), 0666 | IPC_CREAT | IPC_EXCL);

    signal(SIGINT, exit_signal);
    atexit(server_exit);

    printf("Server available, with queue: %d\n", server_queue_ID); 
    while(1){
        handle_message();
    }
    return 0;
}