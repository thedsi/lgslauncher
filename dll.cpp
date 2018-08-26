#include <windows.h>

static BOOL WINAPI MyShell_NotifyIconW(DWORD dwMessage, PNOTIFYICONDATAW lpData)
{
    if (dwMessage == NIM_ADD)
    {
        SetProcessWorkingSetSize(GetCurrentProcess(), (SIZE_T)-1, (SIZE_T)-1);
    }
    return TRUE;
}

static bool Hook(HMODULE hMod)
{
    if (!hMod)
        return false;
    auto base = (char*)hMod;
    auto dos = (IMAGE_DOS_HEADER*)base;
    auto opt = (IMAGE_OPTIONAL_HEADER*)(base + dos->e_lfanew + 4 + sizeof(IMAGE_FILE_HEADER));
    LONG_PTR* iat = (LONG_PTR*)(base + opt->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
    DWORD iat_size = opt->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size / sizeof(LONG_PTR);
    for (DWORD i = 0; i < iat_size; ++i)
    {
        if (iat[i] == (LONG_PTR)&Shell_NotifyIconW)
        {
            DWORD oldProtect = 0;
            BOOL bRet = VirtualProtect(&iat[i], sizeof(LONG_PTR), PAGE_READWRITE, &oldProtect);
            if (bRet)
            {
                iat[i] = (LONG_PTR)&MyShell_NotifyIconW;
                VirtualProtect(&iat[i], sizeof(LONG_PTR), oldProtect, &oldProtect);
                return true;
            }
            break;
        }
    }
    return false;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
        Hook(GetModuleHandleW(L"Qt5Widgets.dll"));
    }
    return TRUE;
}