#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define PORT 5000

int main()
{
    int sock;
    int len;
    int client_len;

    char message[100];

    struct sockaddr_in server;
    struct sockaddr_in client;

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
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Server is waiting...\n");

    // Receive message from client
    client_len = sizeof(client);

    len = recvfrom(sock, message, sizeof(message) - 1, 0,
                   (struct sockaddr *)&client, &client_len);

    if (len == SOCKET_ERROR)
    {
        printf("Receive failed.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Add null character
    message[len] = '\0';

    printf("Message received: %s\n", message);

    // Send same message back to client
    sendto(sock, message, len, 0,
           (struct sockaddr *)&client, client_len);

    printf("Message sent back to client.\n");

    // Close socket
    closesocket(sock);

    // Stop Winsock
    WSACleanup();

    return 0;
}