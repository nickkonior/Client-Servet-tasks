#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "localhost"
#define MAX_INTS 5

int main() {
    int client_fd;
    struct sockaddr_un server_addr;

    // Create socket
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(1);
    }

    // Connect to the server
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected to the server\n");

    char response = 'Y';

    while (response == 'Y' || response == 'y') {
        // Receive the prompt from the server
        char prompt[17];
        if (recv(client_fd, prompt, sizeof(prompt), 0) == -1) {
            perror("recv");
            break;
        }

        // Print the prompt and ask for user input
        printf("%s", prompt);
        scanf(" %c", &response);

        // Send the user response to the server
        if (send(client_fd, &response, sizeof(response), 0) == -1) {
            perror("send");
            break;
        }

        if (response != 'Y' && response != 'y') {
            // Client does not want to proceed
            break;
        }

        int numbers[MAX_INTS];

        // Get input from the user
        printf("Enter %d integers to check if their average is > 20 :\n", MAX_INTS);
        for (int i = 0; i < MAX_INTS; i++) {
            scanf("%d", &numbers[i]);
        }

        // Send the numbers to the server
        for (int i = 0; i < MAX_INTS; i++) {
            if (send(client_fd, &numbers[i], sizeof(int), 0) == -1) {
                perror("send");
                break;
            }
        }

        // Receive and print the result from the server
        char result[128];
        if (recv(client_fd, result, sizeof(result), 0) == -1) {
            perror("recv");
            break;
        }

        printf("Result from the server: %s\n", result);
    }

    // Close the client socket
    close(client_fd);

    return 0;
}
