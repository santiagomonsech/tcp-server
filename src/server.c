#define _POSIX_C_SOURCE 200112L

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>

#define BUFFER_SIZE 1024

const char PORT[] = "8080";

static int is_running = 1;

void handle_sigint(int sig)
{
    is_running = 0;
}

int start_server(int argc, char const *argv[])
{
    signal(SIGINT, handle_sigint);
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    int server_fd;

    int s = getaddrinfo(NULL, PORT, &hints, &result);

    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        server_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (server_fd == -1)
            continue;
        if (bind(server_fd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        close(server_fd);
    }
    freeaddrinfo(result);

    if (rp == NULL)
    {
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    int status = listen(server_fd, 5);
    if (status != 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    struct sockaddr peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);
    while (is_running)
    {
        int client_fd = accept(server_fd, (struct sockaddr *)&peer_addr, &peer_addr_len);
        printf("New connection started %d\n", client_fd);
        if (client_fd == -1)
        {
            if(errno == EINTR) {
                continue;
            }
            perror("accept");
            continue;
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
