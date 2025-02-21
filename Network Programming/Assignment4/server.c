#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024
char response[BUFFER_SIZE];
struct sockaddr_in client_addr;
socklen_t client_addr_len;
int client_socket;
char buffer[BUFFER_SIZE];
struct sockaddr_in server_addr;

void perform_operation() {
time_t rawtime;
struct tm *timeinfo;
    switch (buffer[0]) {
        case 't':
        case 'T':
            // Get current time
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(response, sizeof(response), "%H:%M:%S", timeinfo);
            break;
        case 'd':
        case 'D':
            // Get current date
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(response, sizeof(response), "%Y-%m-%d", timeinfo);
            break;
        case 'a':
        case 'A':
            // Get current date and time
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(response, sizeof(response), "%Y-%m-%d %H:%M:%S", timeinfo);
            break;}
            
}

void create_server_socket(int port) {

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Server_Side > Socket creation failed");
        exit(EXIT_FAILURE);
    }


    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Server_Side > Bind failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

}


void receive_operation(int client_socket) {
    // Receive message from client
	client_addr_len = sizeof(client_addr);
        ssize_t bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0,(struct sockaddr *)&client_addr, &client_addr_len);
        if (bytes_received < 0) {
            perror("Error receiving message from client");
            exit(EXIT_FAILURE);
        }

    printf("Server_Side > Received \"%c\"\n",buffer[0]);
}

void send_result(int client_socket) {
    // Send result back to client
    if (sendto(client_socket, &response, strlen(response), 0,(const struct sockaddr *)&client_addr, client_addr_len) == -1) {
        perror("Server_Side > Send failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server_Side > Sending \"%s\" to the client\n", response);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Server_Side > Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    if (port<=45000 || port>65535){
    	printf("Port should be grater than 45000\n");
        exit(EXIT_FAILURE);
    }

    printf("Server_Side > waiting for client messages.\n");
    create_server_socket(port);
    while (1) {

        receive_operation(client_socket);

        // Check for exit command
        if (buffer[0] == 's' || buffer[0] == 'S') {
            printf("Server_Side > exiting\n");
            close(client_socket);
            break;
        }

        // Perform the operation
        perform_operation();

        // Send result to client
        send_result(client_socket);

        
    }



    return 0;
}
