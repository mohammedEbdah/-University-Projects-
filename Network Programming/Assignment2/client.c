#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define OPERATION_SIZE 100

typedef struct {
    int operand1;
    int operand2;
    char operation;
    int result;
} Operation;

int create_socket(char *server_ip, int server_port) {
    int client_socket;

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;

    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    return client_socket;
}

void send_operation(int client_socket, char operation, Operation *op) {
    // Send operation choice to server
    if (send(client_socket, &operation, sizeof(char), 0) == -1) {
        perror("Send failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Set operation and send to server
    if (send(client_socket, op, sizeof(Operation), 0) == -1) {
        perror("Send failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
}

void receive_result(int client_socket, Operation *op) {
    // Receive result from server
    if (recv(client_socket, op, sizeof(Operation), 0) == -1) {
        perror("Receive failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Client_side > %d %c %d = %d\n", op->operand1, op->operation, op->operand2, op->result);
}

void close_connection(int client_socket) {
    // Close socket
    close(client_socket);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server IP> <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    int client_socket = create_socket(server_ip, server_port);

    char operation;
    Operation op;

    while (1) {
        // Display menu
        printf("1. Add\n2. Mul\n3. Sub\n4. Div\n5. Exit\n");
        printf("Please enter your choice: ");
        scanf(" %c", &operation); // Change to read a single character
        getchar(); // Clear newline from buffer

 

        // Check for exit command
        if (operation == '5'){
            send_operation(client_socket, operation, &op);
            break;}

        // Get operands
        printf("Enter two 16-bit signed operands: ");
        while(1) {
            scanf("%d", &op.operand1);
            if (op.operand1 >= -32768 && op.operand1 <= 32767) {
                break;
            }
            printf("Invalid input. Please enter a valid 16-bit integer (-32768 to 32767): ");
        }
        while(1) {
            scanf("%d", &op.operand2);
            if (op.operand2 >= -32768 && op.operand2 <= 32767) {
                break;
            }
            printf("Invalid input. Please enter a valid 16-bit integer (-32768 to 32767)"); 
        }
        

        // Set operation symbol
        if (operation == '1')
            operation = '+';
        if (operation == '2')
            operation = '*';
        if (operation == '3')
            operation = '-';
        if (operation == '4'){
            operation = '/';
            if (op.operand2 == 0)
            while(1) {
            printf("Invalid input. Divide by 0"); 
            scanf("%d", &op.operand2);
            if (op.operand2 != 0) {
                break;
            }
            
        }
}
        op.operation = operation;
	// Send operation choice to server
        send_operation(client_socket, operation, &op);
        receive_result(client_socket, &op);
    }

    printf("Client_side > exiting\n");

    close_connection(client_socket);

    return 0;
}
