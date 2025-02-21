#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define EXIT_MESSAGE "exit"

void send_tcp_request(const char *hostname, const char *service, const char *input);
void send_udp_request(const char *hostname, const char *service, const char *input);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <hostname> <service>\n", argv[0]);
        exit(1);
    }

    char choice;
    char input[BUFFER_SIZE];

    while (1) {
        printf("**********welcome to NES 416 HW #5 *****************\n");
        printf("Please select your choice\n");
        printf("1- Enter a string to count for a given character using TCP\n");
        printf("2- Enter a string to count for a given character using UDP\n");
        printf("3- Exit\n");
        printf("***************************************************\n");
        scanf(" %c", &choice);

        if (choice == '3') {
            printf("Exiting...\n");
            strcpy(input, EXIT_MESSAGE);
            send_tcp_request(argv[1], argv[2], input);
            send_udp_request(argv[1], argv[2], input);
            break;
        }

        printf("Enter the string: ");
        scanf(" %1023[^\n]", input);  // Read input including spaces

        if (strcmp(input, "-1") == 0) {
            printf("Sending exit message...\n");
            strcpy(input, EXIT_MESSAGE);
            send_tcp_request(argv[1], argv[2], input);
            send_udp_request(argv[1], argv[2], input);
            break;
        }

        switch (choice) {
            case '1':
                send_tcp_request(argv[1], argv[2], input);
                break;
            case '2':
                send_udp_request(argv[1], argv[2], input);
                break;
            default:
                printf("Please try again.\n");
                break;
        }
    }

    return 0;
}

void send_tcp_request(const char *hostname, const char *service, const char *input) {
    struct addrinfo hints, *res;
    int sockfd;
    char buffer[BUFFER_SIZE];
    int num_bytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, service, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(1);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        perror("connect");
        exit(1);
    }

    strcpy(buffer, input);
    send(sockfd, buffer, strlen(buffer), 0);
    num_bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    if (num_bytes < 0) {
        perror("recv");
        exit(1);
    }
    buffer[num_bytes] = '\0';

    if (strcmp(buffer, EXIT_MESSAGE) == 0) {
        printf("exiting...\n");
    } else {
        printf("count: %s\n", buffer);
    }

    close(sockfd);
    freeaddrinfo(res);
}

void send_udp_request(const char *hostname, const char *service, const char *input) {
    struct addrinfo hints, *res;
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_storage server_addr;
    socklen_t addr_len;
    int num_bytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(hostname, service, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(1);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    strcpy(buffer, input);
    addr_len = sizeof server_addr;
    sendto(sockfd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen);
    num_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&server_addr, &addr_len);
    if (num_bytes < 0) {
        perror("recvfrom");
        exit(1);
    }
    buffer[num_bytes] = '\0';

    if (strcmp(buffer, EXIT_MESSAGE) == 0) {
        printf("exiting...\n");
    } else {
        printf("count: %s\n", buffer);
    }

    close(sockfd);
    freeaddrinfo(res);
}

