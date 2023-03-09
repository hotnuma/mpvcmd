#if 0

#include <wtypes.h>

//#include "print.h"

#define MPVPIPE "\\\\.\\pipe\\mpv-pipe"

HANDLE pipeConnect(const wchar_t *wpipename, int timeout)
{
    HANDLE hPipe;
    DWORD access = GENERIC_WRITE;

    for (int i = 0; i < 10; ++i)
    {
        hPipe = ::CreateFileW(wpipename,
            access,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr);

        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        if (::GetLastError() != ERROR_PIPE_BUSY)
        {
            return INVALID_HANDLE_VALUE;
        }

        if (!::WaitNamedPipeW(wpipename, 20000))
        {
            return INVALID_HANDLE_VALUE;
        }

        if (timeout > 0)
            Sleep(timeout);
    }

    DWORD dwMode = PIPE_READMODE_MESSAGE;

    if (!::SetNamedPipeHandleState(
                hPipe,
                &dwMode,
                nullptr,
                nullptr))
    {
        ::CloseHandle(hPipe);

        return INVALID_HANDLE_VALUE;
    }

    return hPipe;
}

void writeMessage(HANDLE hMpvPipe, const char *message)
{
    if (hMpvPipe == INVALID_HANDLE_VALUE)
        return;

    DWORD cbWritten;

    bool ret = ::WriteFile(hMpvPipe,
                           message, strlen(message),
                           &cbWritten, nullptr);
    if (!ret)
    {
        return;
    }
}


#endif


