#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <sys/types.h>
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void print(DWORD errorCode);

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    memset(&si, 0, sizeof(si));
    memset(&pi, 0, sizeof(pi));

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char* paths = strtok(getenv("PATH"),";");
    char* java = "";
    while (paths != NULL) {
        char* path = paths;
        if (strstr(path, "Java") != NULL) {
            java = path;
            break;
        }
        paths = strtok(NULL, ";");
    }

    strcat(java, "/java.exe");
    wchar_t wtext[1024];
    mbstowcs(wtext, java, strlen(java) + 1);
    LPWSTR javaW = wtext;

    if (CreateProcess(javaW,L"java -jar loader.jar", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);

        DWORD exitCode;
        if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
            printf("Child process exited with code %lu\n", exitCode);
        }
        else {
            print(GetLastError());
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        DWORD errorCode = GetLastError();
        print(errorCode);
        return 1;
    }

    if (CreateProcess(javaW, L"java -cp libs/* de.igelstudios.ClientMain", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);

        DWORD exitCode;
        if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
            printf("Child process exited with code %lu\n", exitCode);
        }
        else {
            print(GetLastError());
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        DWORD errorCode = GetLastError();
        print(errorCode);
        return 1;
    }

    return 0;
}

void print(DWORD errorCode) {
    LPSTR errorMessage = NULL;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL,errorCode,MAKELANGID(LANG_NEUTRAL, 
        SUBLANG_DEFAULT),(LPSTR)&errorMessage,0,NULL);

    if (errorMessage) {
        printf("Error: %s\n", errorMessage);
        LocalFree(errorMessage);
    }
}