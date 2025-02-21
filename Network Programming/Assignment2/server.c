#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 55555 // Change this port if needed

typedef struct {
    int operand1;
    int operand2;
    char operation;
    int result;
} Operation;

void perform_operation(Operation *op) {
    switch (op->operation) {
        case '+': // Addition
            op->result = op->operand1 + op->operand2;
            break;
        case '*': // Multiplication
            op->result = op->operand1 * op->operand2;
            break;
        case '-': // Subtraction
            op->result = op->operand1 - op->operand2;
            break;
        case '/': // Division
            if (op->operand2 != 0)
                op->result = op->operand1 / op->operand2;
            else
                op->result = 0; // handle division by zero
            break;
        default:
            printf("Server_Side > Invalid operation\n");
            break;
    }
}

int create_server_socket(int port) {
    int server_socket;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Server_Side > Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Server_Side > Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, 5) == -1) {
        perror("Server_Side > Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server_Side > waiting for client messages.\n");
    return server_socket;
}

int accept_client_connection(int server_socket) {
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Accept connection from client
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) == -1) {
        perror("Server_Side > Accept failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("%s",inet_ntoa(client_addr.sin_addr));

    return client_socket;
}

void receive_operation(int client_socket, char *operation, Operation *op,int port) {
    // Receive operation choice from client
    if (recv(client_socket, operation, sizeof(char), 0) == -1) {
        perror("Server_Side > Receive failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Receive operands from client
    if (recv(client_socket, op, sizeof(Operation), 0) == -1) {
        perror("Server_Side > Receive failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server_Side > Received \"%d, %d\" from the client (127.0.0.1,%d)\n", op->operand1, op->operand2,port);
}

void send_result(int client_socket, Operation *op) {
    // Send result back to client
    if (send(client_socket, op, sizeof(Operation), 0) == -1) {
        perror("Server_Side > Send failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server_Side > Sending \"%d\" to the client\n", op->result);
}

void close_server_socket(int server_socket) {
    // Close server socket
    close(server_socket);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Server_Side > Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);

    int server_socket = create_server_socket(port);

    int client_socket;
    client_socket = accept_client_connection(server_socket);
    while (1) {

        char operation;
        Operation op;

        
	//char x[100] = inet_ntoa(client_addr.sin_addr);
        // Receive operation from client
        receive_operation(client_socket, &operation, &op,port);

        // Check for exit command
        if (operation == '5') {
            printf("Server_Side > exiting\n");
            close(client_socket);
            break;
        }

        // Perform the operation
        perform_operation(&op);

        // Send result to client
        send_result(client_socket, &op);

        
    }

    // Close server socket
    close_server_socket(server_socket);

    return 0;
}
