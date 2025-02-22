// process_states_windows.c
/*
Windows doesn’t expose process states like Linux,
so we simulate similar behaviors with pausing,
resuming, and terminating processes.
*/
#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, "cmd /c timeout 15", NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }

    printf("Child Process (PID: %d) created in SUSPENDED state.\n", pi.dwProcessId);
    Sleep(3000);
    ResumeThread(pi.hThread); // Move process to Running state
    printf("Child process resumed.\n");

    Sleep(5000);
    SuspendThread(pi.hThread); // Move process to Suspended state
    printf("Child process suspended.\n");

    Sleep(5000);
    ResumeThread(pi.hThread); // Move process back to Running state
    printf("Child process resumed again.\n");

    WaitForSingleObject(pi.hProcess, INFINITE);
    printf("Child process finished execution.\n");

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
