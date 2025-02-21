#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>

typedef struct {
    int operand1;
    int operand2;
    char operation;
    int result;
} Operation;

// Reap zombie processes
void sigChld(int signum) {
    while (waitpid(-1, NULL, WNOHANG) > 0); 
}

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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Server_Side > Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    if (port<=45000){
    	printf("Port should be grater than 45000\n");
        exit(EXIT_FAILURE);
    }

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pid_t pidd;
    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Server_Side > Socket creation failed");
        exit(EXIT_FAILURE);
    }

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
    signal(SIGCHLD, sigChld);
       	
        //printf("Received connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), port);
    while (1) {
	
        char operation;
        Operation op;


        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("Server_Side > Accept failed");
            close(server_socket);
            exit(EXIT_FAILURE);
        }
        printf("Connection from client %s:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
	pidd = fork();
        if (pidd == -1) {
            perror("error");
            exit(EXIT_FAILURE);
        }
        if (pidd == 0) {
        close(server_socket);
        for(;;){
        // Receive operation choice from client
        int receee = recv(client_socket, &operation, sizeof(char), 0);
        if (receee == -1) {
            perror("Server_Side > Receive failed");
            close(client_socket);
            continue; // Continue listening for other clients
        }else if(receee ==0){
            printf("%d terminated\n",getpid());
            exit(EXIT_FAILURE);}
	
        //printf("Received \"%c\" from the client (%s, %d)\n", operation, inet_ntoa(client_addr.sin_addr), port);

        


        // Receive operands from client
        if (recv(client_socket, &op, sizeof(Operation), 0) == -1) {
            perror("Server_Side > Receive failed");
            close(client_socket);
            continue; // Continue listening for other clients
        }

	printf("Server_Side > Received \"%d, %d\" from the client (%s, %d)\n", op.operand1, op.operand2, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		        
        // Perform the operation
        perform_operation(&op);

        // Send result back to client
        if (send(client_socket, &op, sizeof(Operation), 0) == -1) {
            perror("Server_Side > Send failed");
            close(client_socket);
            continue; // Continue listening for other clients
        }

        printf("Server_Side > Sending \"%d\" to the client\n", op.result);
	}
         //Close client socket
         close(client_socket);
         exit(EXIT_SUCCESS);}
    else { // Parent process
            close(client_socket);}
}

  close(server_socket);  

return 0;}
