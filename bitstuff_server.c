#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define PORT 5000

// Bit stuffing function
void bitStuff(char input[], char output[])
{
    int i;
    int count = 0;
    int j = 0;

    for (i = 0; input[i] != '\0'; i++)
    {
        output[j] = input[i];
        j++;

        if (input[i] == '1')
            count++;
        else
            count = 0;

        // Insert 0 after five consecutive 1s
        if (count == 5)
        {
            output[j] = '0';
            j++;
            count = 0;
        }
    }

    output[j] = '\0';
}

// Checksum function
int checksum(char data[])
{
    int i;
    int sum = 0;

    for (i = 0; data[i] != '\0'; i++)
    {
        sum = sum + (data[i] - '0');
    }

    return (255 - sum) & 255;
}

int main()
{
    int sock;
    int len;
    int check;
    int client_len;

    char message[100];
    char stuffed[200];

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

    len = recvfrom(sock, message, sizeof(message) - 1, 0,
                   (struct sockaddr *)&client, &client_len);

    if (len == SOCKET_ERROR)
    {
        printf("Receive failed.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    message[len] = '\0';

    printf("String received: %s\n", message);

    // Bit stuffing
    bitStuff(message, stuffed);

    printf("After bit stuffing: %s\n", stuffed);

    // Calculate checksum AFTER bit stuffing
    check = checksum(stuffed);

    printf("Checksum: %d\n", check);

    // Send stuffed data
    sendto(sock, stuffed, strlen(stuffed), 0,
           (struct sockaddr *)&client, client_len);

    // Send checksum
    sendto(sock, (char *)&check, sizeof(check), 0,
           (struct sockaddr *)&client, client_len);

    printf("Stuffed data and checksum sent to client.\n");

    // Close socket
    closesocket(sock);

    // Stop Winsock
    WSACleanup();

    return 0;
}