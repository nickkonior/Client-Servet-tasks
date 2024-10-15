#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "localhost"
#define MAX_INTS 5

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;

    // Create socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }

    // Bind socket to a path
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    unlink(SOCKET_PATH);
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Listen for connections
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Server started. Waiting for connections...\n");

    // Accept and handle client connections
    while (1) {
        client_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == -1) {
            perror("accept");
            continue;
        }

        printf("Client connected\n");

        while (1) {
            // Ask client if they want to proceed
            char response;
            if (send(client_fd, "Proceed? (Y/N): ", 16, 0) == -1) {
                perror("send");
                break;
            }

            if (recv(client_fd, &response, sizeof(response), 0) == -1) {
                perror("recv");
                break;
            }

            if (response != 'Y' && response != 'y') {
                // Client does not want to proceed
                break;
            }

            int numbers[MAX_INTS];

            // Receive N integers from the client
            printf("Receiving %d integers from the client...\n", MAX_INTS);
            for (int i = 0; i < MAX_INTS; i++) {
                if (recv(client_fd, &numbers[i], sizeof(int), 0) == -1) {
                    perror("recv");
                    break;
                }
            }

            // Print the result
            int sum = 0;
            float average;

            for (int i = 0; i < MAX_INTS; i++) {
                sum = sum + numbers[i];
            }
            average = sum / MAX_INTS;

            char result[128];
            if (average > 20) {
                snprintf(result, sizeof(result), "Sequence Ok, average = %f\n", average);
            } else {
                snprintf(result, sizeof(result), "Check Failed\n");
            }

            // Send the result to the client
            if (send(client_fd, result, sizeof(result), 0) == -1) {
                perror("send");
                break;
            }
        }

        // Close the client socket
        close(client_fd);
    }

    // Close the server socket
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}
