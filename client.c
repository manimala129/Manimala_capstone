#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Set server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IP address to binary format
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Connected to server...\n");

    // Receive movie shows from server
    printf("Available movie shows:\n");
    for (int i = 0; i < 4; i++) {
        read(sock, buffer, 1024);
        printf("%s", buffer);
    }

    // Send booking request to server
    printf("Enter your choice (1-4): ");
    int choice;
    scanf("%d", &choice);
    sprintf(buffer, "%d", choice);
    send(sock, buffer, strlen(buffer), 0);

    // Receive booking confirmation from server
    read(sock, buffer, 1024);
    printf("%s\n", buffer);

    // Close socket
    close(sock);
    return 0;
}

