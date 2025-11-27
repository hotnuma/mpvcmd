#include <cstring.h>
#include <clist.h>
#include <libapp.h>

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <print.h>

#define SOCKET_NAME "/tmp/mpvsocket"
//#define NOPANSCAN

int main(int argc, char **argv)
{
    CStringAuto *inpath = cstr_new_size(64);

    if (argc > 1)
        cstr_copy(inpath, argv[1]);

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

    // create first instance
    if (ret == -1)
    {
        close(sock);

        CStringAuto *cmd = cstr_new_size(128);
        cstr_fmt(cmd,
                 "/usr/bin/mpv --idle --no-terminal --force-window --input-ipc-server=%s --",
                 SOCKET_NAME);

        if (!cstr_isempty(inpath))
        {
            cstr_append(cmd, " \"");
            cstr_append(cmd, c_str(inpath));
            cstr_append(cmd, "\"");

            //cmd += strFmt(" \"%s\"", inpath.c_str());
        }

        pexec(c_str(cmd));

        return EXIT_SUCCESS;
    }

    // send load message to existing instance
    if (!cstr_isempty(inpath))
    {
        CStringAuto *message = cstr_new_size(128);

#ifdef NOPANSCAN
        cstr_fmt(message, "loadfile \"%s\" replace\n",
                 c_str(inpath));
#else
        cstr_fmt(message, "loadfile \"%s\" replace panscan=0.0\n",
                 c_str(inpath));
#endif

        ret = write(sock, c_str(message), cstr_size(message) + 1);

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


