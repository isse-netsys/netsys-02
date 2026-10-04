#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    char hostname[256];
    struct addrinfo hints, *result, *p;
    char ip[INET6_ADDRSTRLEN];

    gethostname(hostname, sizeof(hostname));
    printf("Local hostname: %s\n", hostname);

    const char *target =
        (argc > 1) ? argv[1] : "example.com";

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;

    if (getaddrinfo(target, NULL, &hints, &result) != 0) {
        printf("Could not resolve host\n");
        return 1;
    }

    printf("Addresses for %s:\n", target);

    for (p = result; p != NULL; p = p->ai_next) {

        void *addr;

        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 =
                (struct sockaddr_in *)p->ai_addr;

            addr = &ipv4->sin_addr;
        } else {
            struct sockaddr_in6 *ipv6 =
                (struct sockaddr_in6 *)p->ai_addr;

            addr = &ipv6->sin6_addr;
        }

        inet_ntop(p->ai_family, addr, ip, sizeof(ip));

        printf("  %s\n", ip);
    }

    freeaddrinfo(result);

    return 0;
}
