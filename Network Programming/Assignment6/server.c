#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <errno.h>

#define MAXLINE 1024
struct sockaddr_in client_addr;
socklen_t client_len = sizeof(client_addr);
int server_socket;            // Server socket descriptor
struct addrinfo *res;         // Address info structure
int connectionFromClient = 0; // Flag to indicate if a client connection has been established

// Signal handler for SIGCHLD to reap zombie processes
void sigchld_handler(int signum) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// Function to send the result back to the client
void send_result(int client_socket, char *operation) {
    if (send(client_socket, operation, strlen(operation), 0) == -1) {
        perror("Server_Side > Send failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    printf("Server_Side > Sending \"%s\" to the client\n", operation);
}

// Function to perform the operation on the received data
void perform_operation(int client_socket, char *operation, ssize_t n) {
    operation[n] = '\0'; // Null-terminate the received string
    char character = operation[0]; // First character is the one to count
    char *substring = operation + 1; // Rest of the string
    int count = 0;

    // Count occurrences of the character in the string
    while (*substring) {
        if (*substring == character) count++;
        substring++;
    }

    // Prepare the result string
    snprintf(operation, MAXLINE, "%d", count);
    send_result(client_socket, operation);
}

// Function to receive operation from the client
void receive_operation(int client_socket, char *operation, int port, int pid) {
    ssize_t n = recv(client_socket, operation, MAXLINE, 0);

    if (n == -1) {
        perror("Server_Side > Receive failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server_Side > Received \"%s\" from the client ((127.0.0.1),%d)\n", operation, ntohs(client_addr.sin_port));
    perform_operation(client_socket, operation, n);
}

// Thread function to handle client requests
void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    free(arg);
    char operation[MAXLINE];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if (getsockname(server_socket, (struct sockaddr *)&addr, &addr_len) == 0) {
        receive_operation(client_socket, operation, ntohs(addr.sin_port), getpid());
    } else {
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    close(client_socket); // Close client socket
    return NULL;
}

// Function to create and set up the server socket
int create_server_socket(char *port) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    // Ensure the port is in the valid range
    if (ntohs(((struct sockaddr_in*)(res->ai_addr))->sin_port) < 1024 || ntohs(((struct sockaddr_in*)(res->ai_addr))->sin_port) > 65535) {
        printf("Port should be between 1024 and 65535\n");
        exit(0);
    }

    // Create the server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Server_Side > Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the address and port
    if (bind(server_socket, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Server_Side > Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == -1) {
        perror("Server_Side > Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    return server_socket;
}

// Function to accept client connections
void accept_client_connection(int server_socket) {
    
    int *client_socket = malloc(sizeof(int));

    if ((*client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) < 0) {
        if (errno == EINTR) {
            return;
        } else {
            perror("Server_Side > Accept failed");
            close(server_socket);
            free(client_socket);
            exit(EXIT_FAILURE);
        }
    }

    // Print the first client connection message
    if (connectionFromClient == 0) {
        printf("Connection from client (127.0.0.1,%d)\n", ntohs(client_addr.sin_port));
        connectionFromClient++;
    }

    // Create a new thread to handle the client
    pthread_t tid;
    if (pthread_create(&tid, NULL, handle_client, client_socket) != 0) {
        perror("Server_Side > Pthread create failed");
        free(client_socket);
    }

    pthread_detach(tid); // Detach the thread
}

// Function to close the server socket
void close_server_socket(int server_socket) {
    close(server_socket);
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Server_Side > Usage: %s <service_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *port = argv[1];
    server_socket = create_server_socket(port);

    printf("Server_Side > waiting for clients messages.\n");
    while (1) {
        accept_client_connection(server_socket);
    }

    freeaddrinfo(res);
    close_server_socket(server_socket);
    return 0;
}
