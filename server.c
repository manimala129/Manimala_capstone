#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

// Structure to represent a movie show
typedef struct {
    char time[10];
    char movie[20];
    int price;
} MovieShow;

// Array of movie shows
MovieShow shows[] = {
    {"10:00", "Avengers", 150},
    {"2:00", "Spider-Man", 200},
    {"6:00", "The Lion King", 250},
    {"9:00", "Joker", 300}
};

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Set address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted...\n");

        // Send movie shows to client
        char buffer[1024] = {0};
        for (int i = 0; i < 4; i++) {
            sprintf(buffer, "%s - %s (rs%d)\n", shows[i].time, shows[i].movie, shows[i].price);
            send(new_socket, buffer, strlen(buffer), 0);
        }

        // Receive booking request from client
        read(new_socket, buffer, 1024);
        int choice = atoi(buffer);

        // Process booking request
        if (choice >= 1 && choice <= 4) {
            sprintf(buffer, "Booking successful for %s - %s (rs%d)\n", shows[choice - 1].time, shows[choice - 1].movie, shows[choice - 1].price);
            send(new_socket, buffer, strlen(buffer), 0);
        } else {
            sprintf(buffer, "Invalid choice\n");
            send(new_socket, buffer, strlen(buffer), 0);
        }

        // Close socket
        close(new_socket);
    }

    return 0;
}

