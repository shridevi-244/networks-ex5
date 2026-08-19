#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define PORT 5000

int main()
{
    int sock;
    int len;

    char message[100];
    char buffer[100];

    struct sockaddr_in server;

    WSADATA wsa;

    // Start Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock == INVALID_SOCKET)
    {
        printf("Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Get message from user
    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);

    // Remove newline
    message[strcspn(message, "\n")] = '\0';

    // Send message to server
    sendto(sock, message, strlen(message), 0,
           (struct sockaddr *)&server, sizeof(server));

    printf("Message sent to server.\n");

    // Receive echoed message
    len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                   NULL, NULL);

    if (len == SOCKET_ERROR)
    {
        printf("Receive failed.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Add null character
    buffer[len] = '\0';

    printf("Echo from server: %s\n", buffer);

    // Close socket
    closesocket(sock);

    // Stop Winsock
    WSACleanup();

    return 0;
}