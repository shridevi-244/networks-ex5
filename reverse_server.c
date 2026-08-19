#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define PORT 5000

int main()
{
    int sock;
    int len;
    int i;
    int client_len;

    char str[100];
    char temp;

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

    // Receive string from client
    client_len = sizeof(client);

    len = recvfrom(sock, str, sizeof(str) - 1, 0,
                   (struct sockaddr *)&client, &client_len);

    if (len == SOCKET_ERROR)
    {
        printf("Receive failed.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    str[len] = '\0';

    printf("String received: %s\n", str);

    // Reverse the string
    for (i = 0; i < len / 2; i++)
    {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }

    printf("Reversed string: %s\n", str);

    // Send reversed string to client
    sendto(sock, str, strlen(str), 0,
           (struct sockaddr *)&client, client_len);

    printf("Reversed string sent to client.\n");

    // Close socket
    closesocket(sock);

    // Stop Winsock
    WSACleanup();

    return 0;
}

