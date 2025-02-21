#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int BUFFERSIZE1;
int BUFFERSIZE2;
struct sockaddr_in server_addr;
void set_buffer_sizes(int sockfd) {
    int snd_buf, rcv_buf;
    printf("Enter new sending buffer size: ");
    if (scanf("%d", &snd_buf)!=1)
    	exit(EXIT_FAILURE);
    BUFFERSIZE1 = snd_buf;
    printf("Enter new receiving buffer size: ");
    if (scanf("%d", &rcv_buf)!=1)
    	exit(EXIT_FAILURE);
    BUFFERSIZE2 = rcv_buf;
    // Set sending buffer size
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &snd_buf, sizeof(snd_buf));
    // Set receiving buffer size
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_buf, sizeof(rcv_buf));

    printf("New buffer sizes set.\n");
}

int create_socket(char *server_ip, int server_port) {
    int client_socket;

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }


    memset(&server_addr, 0, sizeof(server_addr));
    // Prepare the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton error");
        exit(1);
    }

    printf("Connected to server\n");

    return client_socket;
}

void send_operation(int client_socket, char operation) {
    // Send operation choice to server
    if (sendto(client_socket, &operation, BUFFERSIZE1, 0,(struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        perror("Send failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
}

ssize_t receive_result(int client_socket) {
    char buffer[BUFFERSIZE2];
    // Receive result from server
    ssize_t bytes_received = recvfrom(client_socket, buffer, BUFFERSIZE2, 0, NULL, NULL);
    if (bytes_received == -1) {
        perror("Receive failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    // Null terminate the received data to use it as string
    buffer[bytes_received] = '\0';
    printf("Server response: %.*s\n", (int)bytes_received, buffer);
    return bytes_received;
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server IP> <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    if (server_port<=45000 || server_port>65535){
    	printf("Port should be grater than 45000\n");
        exit(EXIT_FAILURE);
    }
    int client_socket = create_socket(server_ip, server_port);

    char operation;
    // Prompt the user to set new buffer sizes
    set_buffer_sizes(client_socket);
    while (1) {
        char choice;
        // Display menu
        printf("******** Welcome to NES 411 HW #1 ***********\n");
        printf("1- Insert t|T for time\n");
        printf("2- Insert d|D for date\n");
        printf("3- Insert a|A for date+time\n");
        printf("4- Insert s|S to quit\n");
        printf("************************************************\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice); // Change to read a single character
        getchar(); // Clear newline from buffer
	
	if (choice != 't' && choice != 'd' && choice != 'a' && choice != 'T' && choice != 'A' && choice != 'D' && choice != 's' && choice != 'S'){
	printf("invalid choice\n");
		continue;}
        send_operation(client_socket, choice);
        if (choice == 's' || choice == 'S'){
        	printf("Client_side > exiting\n");
		break;}
	        
        receive_result(client_socket);
        }
        
      

    return 0;
}
