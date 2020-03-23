# Reverse shell DLL

Accompanying program: dllmain.cpp

## Overview

By creating our own DLL, we can have greater control over debugging issues in attacks such as DLL injection.

On a high level, what we are doing is:
1. Initializing winsock
  - WSAStartup()
2. Creating and configuring a socket and connection details
  - WSASocket()
  - Filling out the sockaddr_in struct
3. Connecting to the remote server which we want to give a shell to
  - WSAConnect()
4. Creating a process which is a shell, and whose I/O goes over the socket we created (allowing for the remote control)
  - Filling out specialized startup info and process info
  - CreateProcess()
5. Waiting for the shell to close
  - WaitForSingleObject()
6. Cleaning up after ourselves
  - Closing the handles to the remote process and its thread
  - Closing the socket
  - WSACleanup()

## Notes
By mixing and matching existing code, fixing compiler errors, and testing the code, we can create our very own payload from scratch.

If you want to get more into the actual implementation details and Windows internals (which I recommend), here are the relevant MSDN links:
- WSAStartup(): https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
- WSASocket(): https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsasocketa
- sockaddr/sockaddr_in struct: https://docs.microsoft.com/en-us/windows/win32/winsock/sockaddr-2
- in_addr struct (part of sockaddr_in): https://docs.microsoft.com/en-us/windows/win32/api/winsock2/ns-winsock2-in_addr
- Converting text IPv4 address into binary representation (for sockaddr_in.sin_addr): https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inetptonw
- htons(): https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-htons
- WSAConnect(): https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsaconnect
- STARTUPINFO struct: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfoa
- PROCESS_INFORMATION struct: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-process_information
- CreateProcess(): https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa
- WaitForSingleObject(): https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
- CloseHandle(): https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
- closesocket(): https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-closesocket
- WSACleanup(): https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsacleanup
- Working with strings: https://docs.microsoft.com/en-us/windows/win32/learnwin32/working-with-strings