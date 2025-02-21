#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define EXIT_MESSAGE "exit"

fd_set master_set;  // Declare master_set globally
int running = 1;

void handle_tcp_client(int client_sock);
void handle_udp_request(int udp_sock);
void sigchld_handler(int s);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <service_name>\n", argv[0]);
        exit(1);
    }

    struct addrinfo hints, *res;
    int tcp_sock, udp_sock, new_sock;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    fd_set read_fds;
    int fdmax, i;
    struct sigaction sa;

    // Resolve service name
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, argv[1], &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(1);
    }

    // Create TCP socket
    tcp_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (tcp_sock < 0) {
        perror("TCP socket");
        exit(1);
    }

    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    // Bind TCP socket
    if (bind(tcp_sock, res->ai_addr, res->ai_addrlen) < 0) {
        perror("TCP bind");
        exit(1);
    }

    // Listen on TCP socket
    if (listen(tcp_sock, 10) < 0) {
        perror("TCP listen");
        exit(1);
    }

    // Create UDP socket
    udp_sock = socket(res->ai_family, SOCK_DGRAM, 0);
    if (udp_sock < 0) {
        perror("UDP socket");
        exit(1);
    }

    // Bind UDP socket
    if (bind(udp_sock, res->ai_addr, res->ai_addrlen) < 0) {
        perror("UDP bind");
        exit(1);
    }

    freeaddrinfo(res);

    // Set up signal handler for reaping zombie processes
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; // Restart interrupted system calls
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // Initialize master set and add sockets
    FD_ZERO(&master_set);
    FD_SET(tcp_sock, &master_set);
    FD_SET(udp_sock, &master_set);
    fdmax = (tcp_sock > udp_sock) ? tcp_sock : udp_sock;

    printf("Server is running and listening...\n");

    while (running) {
        read_fds = master_set;

        int select_result = select(fdmax + 1, &read_fds, NULL, NULL, NULL);
        if (select_result == -1) {
            if (errno == EINTR) {
                continue; // Ignore interrupted system call and restart select
            } else {
                perror("select");
                exit(1);
            }
        }

        for (i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == tcp_sock) {
                    // Handle new TCP connection
                    addr_size = sizeof client_addr;
                    new_sock = accept(tcp_sock, (struct sockaddr *)&client_addr, &addr_size);
                    
                    if (new_sock < 0) {
                    if (errno==EINTR)
            	continue;
            	else 
                        perror("accept");
                    } else {
                        if (fork() == 0) { // Child process
                            close(tcp_sock); // Child doesn't need the listener
                            handle_tcp_client(new_sock);
                            close(new_sock);
                            exit(0);
                        } else { // Parent process
                            close(new_sock); // Parent doesn't need this
                        }
                    }
                } else if (i == udp_sock) {
                    // Handle UDP request
                    handle_udp_request(udp_sock);
                }
            }
        }
    }

    printf("Server is shutting down...\n");
    close(tcp_sock);
    close(udp_sock);
    return 0;
}

void handle_tcp_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    int num_bytes;
    char target_char;
    int count = 0;

    printf("Handling TCP client...\n");
    num_bytes = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);
    if (num_bytes < 0) {
        perror("recv");
        return;
    }
    buffer[num_bytes] = '\0';

    if (strcmp(buffer, EXIT_MESSAGE) == 0) {
        running = 0;
        printf("TCP Server :Exit message.\n");
        strcpy(buffer, EXIT_MESSAGE);
        send(client_sock, buffer, strlen(buffer), 0);
        FD_ZERO(&master_set); // Clear the master set to stop select from monitoring
        return;
    }

    target_char = buffer[0];
    for (int i = 1; i < num_bytes; i++) {
        if (buffer[i] == target_char) {
            count++;
        }
    }
    sprintf(buffer, "%d", count);
    if (send(client_sock, buffer, strlen(buffer), 0) < 0) {
        perror("send");
    }
    
}

void handle_udp_request(int udp_sock) {
    char buffer[BUFFER_SIZE];
    struct sockaddr_storage client_addr;
    socklen_t addr_len = sizeof client_addr;
    int num_bytes;
    char target_char;
    int count = 0;

    printf("Handling UDP request...\n");
    num_bytes = recvfrom(udp_sock, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&client_addr, &addr_len);
    if (num_bytes < 0) {
        perror("recvfrom");
        return;
    }
    buffer[num_bytes] = '\0';

    if (strcmp(buffer, EXIT_MESSAGE) == 0) {
        running = 0;
        printf("UDP Server :Exit message.\n");
        strcpy(buffer, EXIT_MESSAGE);
        if (sendto(udp_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, addr_len) < 0) {
            perror("sendto");
        }
        FD_ZERO(&master_set); // Clear the master set to stop select from monitoring
        return;
    }

    target_char = buffer[0];
    for (int i = 1; i < num_bytes; i++) {
        if (buffer[i] == target_char) {
            count++;
        }
    }
    sprintf(buffer, "%d", count);
    if (sendto(udp_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, addr_len) < 0) {
        perror("sendto");
    } 
}

void sigchld_handler(int s) {
    (void)s; // Quiet unused variable warning
    // Wait for all dead processes
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

