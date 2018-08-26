#include <windows.h>

static UINT Main()
{
    const wchar_t dll[] = L"lgslauncher.dll";
    wchar_t path[MAX_PATH];

    DWORD dwRet = GetModuleFileNameW(nullptr, path, MAX_PATH);
    if (dwRet == 0 || dwRet >= MAX_PATH)
        return 1;
    wchar_t* lastSlash = nullptr;
    for (wchar_t* p = path; *p; ++p)
        if (*p == L'\\' || *p == L'/')
            lastSlash = p;
    if (!lastSlash)
        lastSlash = path;
    else
        ++lastSlash;
    DWORD len = DWORD((lastSlash - path) + (sizeof(dll) / sizeof(*dll)));
    if (len >= MAX_PATH)
        return 2;
    lstrcpyW(lastSlash, dll);
    wchar_t cmd[] = LR"("C:\Program Files\Logitech Gaming Software\LCore.exe" /minimized)";
    STARTUPINFOW si = {};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi;
    BOOL bRet = CreateProcessW(nullptr, cmd, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &si, &pi);
    if (!bRet)
        return 3;
    UINT result;
    void* mem = VirtualAllocEx(pi.hProcess, nullptr, len * sizeof(*path), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!mem)
        result = 4;
    else
    {
        bRet = WriteProcessMemory(pi.hProcess, mem, path, len * sizeof(*path), nullptr);
        if (!bRet)
            result = 5;
        else
        {
            QueueUserAPC((PAPCFUNC)&LoadLibraryW, pi.hThread, (ULONG_PTR)mem);
            result = 0;
        }
    }
    if (result != 0)
    {
        TerminateProcess(pi.hProcess, result);
    }
    else
    {
        ResumeThread(pi.hThread);
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return result;
}

void CALLBACK Start()
{
    ExitProcess(Main());
}