#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

typedef struct {
    int operand1;
    int operand2;
    char operation;
    int result;
} Operation;

int client_socket;
void sigQuit(int sig) {
    printf("Exiting.\n");
    close(client_socket);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server IP> <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        exit(1);
    }
    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");
    signal(SIGQUIT, sigQuit);
    int operation;
    Operation op;

    while (1) {
        // Display menu
        printf("1. Add\n2. Mul\n3. Sub\n4. Div\nSIGQUIT\n");
        printf("Please enter your choice: ");
        scanf("%d", &operation); // Change to read a single character
	if (operation != 5 && operation != 4 && operation != 3 && operation != 2 && operation != 1){
		printf("Invalid choice\n");
		while (getchar() != '\n');
		continue;}
        // Send operation choice to server
        if (send(client_socket, &operation, sizeof(char), 0) == -1) { // Send a single character
            perror("Send failed");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

	
        // Get operands
        printf("Enter two 16-bit signed operands: ");
        for (;;) {
        if (scanf("%d", &op.operand1)== 1 && op.operand1 >= -32768 && op.operand1 <= 32767) {
            break;
        }
        while (getchar() != '\n');
        printf("Invalid input. Please enter a valid 16-bit integer (-32768 to 32767): ");
    }
    for (;;) {
        if (scanf("%d", &op.operand2)== 1 && op.operand2 >= -32768 && op.operand2 <= 32767) {
            break;
        }
        while (getchar() != '\n');
        printf("Invalid input. Please enter a valid 16-bit integer (-32768 to 32767): ");
    }
    	if (op.operand2==0 && operation == '4')
        for (;;) {
        if (op.operand2!=0) {
            break;
        }
        printf("Invalid input. divide by zero ");
        while (scanf("%d", &op.operand2)!=1){
        printf("Invalid input. Please enter a valid 16-bit integer (-32768 to 32767): ");
        while (getchar() != '\n');}
    }
    
        getchar(); // Clear newline from buffer
        switch(operation){
	case 1: operation='+'; break;
	case 2: operation='*'; break;
	case 3: operation='-'; break;
	case 4: operation='/'; break;}

			
        // Set operation and send to server
        op.operation = operation;
        if (send(client_socket, &op, sizeof(Operation), 0) == -1) {
            perror("Send failed");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        // Receive result from server
        if (recv(client_socket, &op, sizeof(Operation), 0) == -1) {
            perror("Receive failed");
            close(client_socket);
            exit(EXIT_FAILURE);
        }
	
	if (op.result > 32767 || op.result < -32768) {
        printf("Error: Result exceeds the range of 16-bit integer.\n");
        continue;
    }
    
        printf("Client_side > %d %c %d = %d (%s, %d)\n", op.operand1, op.operation, op.operand2, op.result, server_ip, server_port);
    }

    printf("Client_side > exiting\n");

    // Close socket
    close(client_socket);

    return 0;
}
