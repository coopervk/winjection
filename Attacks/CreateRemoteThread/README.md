# CreateRemoteThread

Accompanying program: CreateRemoteThread\CreateRemoteThread.vbs

## Overview

By creating a thread and mapping shellcode into the memory of another process using standard WinAPI functions, we can execute arbitrary shellcode as another process given that we have proper permissions to said process.

1. OpenProcess(): Open a handle to the target process
2. VirtualAllocEx(): Allocate memory in the target process for your shellcode
3. WriteProcessMemory(): Map the shellcode into the memory you allocated in step 2
4. CreateRemoteThread(): Create a thread on the remote process which begins execution at your shellcode.
5. CloseHandle(): Optional, good practice. 

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
- CloseHandle(): https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle