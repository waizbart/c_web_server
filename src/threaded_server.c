#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

#define DEFAULT_PORT 8081
#define DEFAULT_BUFLEN 1024

DWORD WINAPI handle_client(LPVOID lpParam)
{   
    SOCKET clientSocket = (SOCKET)lpParam;
    char buffer[DEFAULT_BUFLEN];

    int recvLen = recv(clientSocket, buffer, DEFAULT_BUFLEN, 0);
    if (recvLen > 0)
    {
        buffer[recvLen] = '\0';
        printf("Mensagem recebida do cliente: %s\n", buffer);

        // Esperar um pouco para simular um processamento mais demorado
        Sleep(1000);

        // Enviar uma resposta simples de volta ao cliente
        const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, world! Servidor multi-thread em C</h1></body></html>";
        send(clientSocket, response, strlen(response), 0);
    }

    closesocket(clientSocket);

    return 0;
}

int main()
{
    WSADATA wsaData;
    SOCKET listenSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char recvBuf[DEFAULT_BUFLEN];
    int recvLen;

    // Inicializar o Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Falha ao inicializar o Winsock.\n");
        return 1;
    }

    // Criar um socket para escutar conexões de entrada
    if ((listenSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Erro ao criar o socket de escuta.\n");
        WSACleanup();
        return 1;
    }

    // Configurar o endereço do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(DEFAULT_PORT);

    // Associar o socket à porta e endereço local
    if (bind(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        printf("Erro ao associar o socket à porta.\n");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Colocar o socket em estado de escuta
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("Erro ao colocar o socket em estado de escuta.\n");
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    printf("Servidor web multi-thread em execucao na porta %d...\n", DEFAULT_PORT);

    // Aceitar conexões de entrada
    while (1)
    {
        // Aguardar por uma conexão
        clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET)
        {
            printf("Erro ao aceitar a conexão do cliente.\n");
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        // Create a new thread to handle client request
        HANDLE hThread = CreateThread(NULL, 0, handle_client, (LPVOID)clientSocket, 0, NULL);
        if (hThread == NULL)
        {
            printf("Erro ao criar a thread para lidar com a requisição do cliente.\n");
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }
        CloseHandle(hThread);
    }

    // Fechar o socket de escuta
    closesocket(listenSocket);
    WSACleanup();
    return 0;
}
