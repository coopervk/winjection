# CreateRemoteThread

Accompanying program: dllmain.cpp

## Overview

By creating our own DLL, we can have greater control over debugging issues in attacks such as DLL injection.

On a high level, what we are doing is:
1. Initializing winsock
  - WSAStartup
2. Creating and configuring a socket and connection details
  - WSASocket
  - Filling out the sockaddr_in struct
3. Connecting to the remote server which we want to give a shell to
  - WSAConnect
4. Creating a process which is a shell, and whose I/O goes over the socket we created (allowing for the remote control)
  - Filling out specialized startup info and process info
  - CreateProcess
5. Waiting for the shell to close
  - WaitForSingleObject
6. Cleaning up after ourselves
  - Closing the handles to the remote process and its thread
  - Closing the socket
  - WSACleanup

## Details

CreateRemoteThread is a WinAPI function which creates a thread which runs in the virtual address space of another process.
This is done with the handle of the other process, which requires one to have the correct rights:
1. PROCESS_CREATE_THREAD
2. PROCESS_QUERY_INFORMATION
3. PROCESS_VM_OPERATION
4. PROCESS_VM_WRITE
5. PROCESS_VM_READ

Other parameters for CreateRemoteThread include:
- lpThreadAttributes
  + For specifying a security descriptor for the new thread and whether child processes inherit the returned handle
  + By default (NULL), you get a default security descriptor and the handle is not inheritble by child processes.
    * The default security descriptor is the Access Control Entries which are inheritable if in a hierarchy
    * Otherwise from the primary or impersonation token of the creator, which is allow generic all to own process and allow generic all to local system
- dwStackSize
  + The initial size of the stack in bytes (automatically page rounded), with 0 as a default.
- lpStartAddress
  +  A pointer to the application's (shellcode's) LPTHREAD_START_ROUTINE
See Raymond Chen "Understanding defualt security descriptors" in sources.

## Notes

While creating the remote thread is the kicker, just remember to understand the other basic Windows API calls as well:
- OpenProcess(): https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess
- VirtualAllocEx(): https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualallocex
- WriteProcessMemory(): https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory
- WaitForSingleObject(): https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
- VirtualAllocFreeEx(): https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualfreeex
- CloseHandle(): https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle