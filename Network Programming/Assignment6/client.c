#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>

#define MAXLINE 1024

// Structure to hold arguments for each thread
typedef struct {
    char *server_ip;
    char *server_port;
    char *substring;
    char operation;
    int result;
} ThreadArgs;

// Function to receive the result from the server
void receive_result(int client_socket, ThreadArgs *args) {
    char operation[MAXLINE];
    ssize_t n = recv(client_socket, operation, MAXLINE, 0);
    if (n == -1) {
        perror("Receive failed");
        close(client_socket);
        pthread_exit(NULL);
    }
    operation[n] = '\0';
    args->result = atoi(operation);
}

// Function to send the operation to the server and receive the result
void send_operation(int client_socket, char *operation, ThreadArgs *args) {
    if (send(client_socket, operation, strlen(operation), 0) == -1) {
        perror("Send failed");
        close(client_socket);
        pthread_exit(NULL);
    }
    receive_result(client_socket, args);
}

// Function to create a socket and communicate with the server
void create_socket(ThreadArgs *args) {
    struct addrinfo hints, *res;
    int client_socket;
    char buffer[MAXLINE];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Resolve server address and port
    if (getaddrinfo(args->server_ip, args->server_port, &hints, &res) != 0){
        perror("getaddrinfo");
        close(client_socket);
        freeaddrinfo(res);
        pthread_exit(NULL);
    }
    
    // Check if port number is within valid range
    if (ntohs(((struct sockaddr_in*)(res->ai_addr)) ->sin_port)<1024 || ntohs(((struct sockaddr_in*)(res->ai_addr)) ->sin_port)>65535){
        printf("Port should be between 1024 and 65535\n");
        exit(0);
    }

    // Get server host by name
    struct hostent *server_host = gethostbyname(args->server_ip);
    if ((client_socket = socket(AF_INET, SOCK_STREAM, res->ai_protocol)) == -1) {
        perror("Socket creation failed");
        close(client_socket);
        freeaddrinfo(res);
        pthread_exit(NULL);
    }

    // Connect to the server
    if (connect(client_socket, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Connection failed");
        close(client_socket);
        freeaddrinfo(res);
        pthread_exit(NULL);
    }

    // Prepare the message to send to the server
    snprintf(buffer, MAXLINE, "%c%s", args->operation, args->substring);
    send_operation(client_socket, buffer, args);

    // Close the socket and free address info
    close(client_socket);
    freeaddrinfo(res);
}

// Thread function to create a socket and communicate with the server
void *client_thread(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    create_socket(args);
    return NULL;
}

int main(int argc, char *argv[]) {
    // Check for proper usage
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server IP> <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    char *server_port = argv[2];
    char operation;
    char operationString[MAXLINE];
    int numThreads;

    // Get input from the user
    printf("Enter the string: ");
    scanf("%[^\n]%*c", operationString);
    printf("Enter the character to search for: ");
    scanf(" %c", &operation);
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("Enter number of required threads: ");
    while (scanf("%d", &numThreads) != 1){
        printf("Please enter valid integer number: ");
        while (getchar() != '\n');
    }

    pthread_t threads[numThreads];
    ThreadArgs thread_args[numThreads];
    int segment_size = strlen(operationString) / numThreads;
    int total_count = 0;

    // Create threads and distribute the string segments among them
    for (int i = 0; i < numThreads; i++) {
        int start_index = i * segment_size;
        int end_index = (i == numThreads - 1) ? strlen(operationString) : (i + 1) * segment_size;
        thread_args[i].server_ip = server_ip;
        thread_args[i].server_port = server_port;
        thread_args[i].operation = operation;
        thread_args[i].substring = strndup(operationString + start_index, end_index - start_index);
        thread_args[i].result = 0;
        pthread_create(&threads[i], NULL, client_thread, &thread_args[i]);
    }

    // Wait for threads to finish and accumulate results
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
        total_count += thread_args[i].result;
        free(thread_args[i].substring);
    }

    // Print the total count of occurrences
    printf("Total occurrences of '%c': %d\n", operation, total_count);

    return 0;
}
