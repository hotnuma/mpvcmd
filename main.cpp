#include <CString.h>
#include <CList.h>
#include <libapp.h>

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <print.h>

#define SOCKET_NAME "/tmp/mpvsocket"

int main(int argc, char **argv)
{
    CString inpath;

    if (argc > 1)
        inpath = argv[1];

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    int ret = connect(sock, (const struct sockaddr*) &addr, sizeof(addr));
    if (ret == -1)
    {
        close(sock);

        CString cmd = strFmt("/usr/bin/mpv --idle --no-terminal --force-window --input-ipc-server=%s --",
                             SOCKET_NAME);

        if (!inpath.isEmpty())
        {
            cmd += strFmt(" \"%s\"", inpath.c_str());
        }

        pexec(cmd);

        return EXIT_SUCCESS;
    }

    if (!inpath.isEmpty())
    {
        CString message = strFmt("loadfile \"%s\" replace panscan=0.0\n",
                                 inpath.c_str());

        ret = write(sock, message.c_str(), message.size() + 1);

        if (ret == -1)
        {
            perror("write");
            close(sock);

            return EXIT_FAILURE;
        }
    }

    close(sock);

    return EXIT_SUCCESS;
}


