# DLL Injection

Accompanying program: InjectDLL.cpp

## Overview

By calling LoadLibrary in a remote process we can force it to load a DLL and execute it.

1. OpenProcess(): Open a handle to the target process
2. VirtualAllocEx(): Allocate memory in the target process for a call to LoadLibrary
3. WriteProcessMemory(): Write the path of the DLL to be injected into the allocated space
4. GetProcAddress(GetModuleHandle()): Get the address of the wide version of LoadLibrary, LoadLibraryW, and cast it to a PThread start routine for execution
5. CreateRemoteThread(): Create a thread on the remote process which invokes LoadLibraryW on the path of the DLL
6. WaitForSingleObject(): Wait for the thread to complete (DLL to fully load)
7. VirtualAllocFreeEx(): Free the memory we allocated in the remote process since we no longer need it
8. CloseHandle(): Close the handle we had to the object since we no longer need it

## Details

This is particularly similar to the direct shellcode attack in the previous CreateRemoteThread attack.
The key difference is that we are loading an entire DLL rather than just executing some premade shellcode.
This is particularly useful considering we can write real code that can execute different, specific code in different contexts, such as every time a new thread starts in the process, just when loading the DLL, etc.
This makes DLL injection a slightly more sophisticated, customizable attack, suitable for everything from basic game hacks to password stealing.

## Notes

Always know your Windows API calls!
- OpenProcess(): https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess
- VirtualAllocEx(): https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualallocex
- WriteProcessMemory(): https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory
- GetProcAddress(): https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress
- GetModuleHandle(): https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlea
- WaitForSingleObject(): https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
- VirtualAllocFreeEx(): https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualfreeex
- CloseHandle(): https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle