#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define PORT 5000

int main()
{
    int sock;
    int len;

    char str[100];
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

    // Get string from user
    printf("Enter string: ");
    fgets(str, sizeof(str), stdin);

    // Remove newline
    str[strcspn(str, "\n")] = '\0';

    // Send string to server
    sendto(sock, str, strlen(str), 0,
           (struct sockaddr *)&server, sizeof(server));

    printf("String sent to server.\n");

    // Receive reversed string
    len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                   NULL, NULL);

    if (len == SOCKET_ERROR)
    {
        printf("Receive failed.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    buffer[len] = '\0';

    printf("Reversed string from server: %s\n", buffer);

    // Close socket
    closesocket(sock);

    // Stop Winsock
    WSACleanup();

    return 0;
}