#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(void)
{
    unsigned char query[512] = {0};
    unsigned char response[512];

    int pos = 12;

    /* DNS header */
    query[0] = 0x12;
    query[1] = 0x34;   // Transaction ID

    query[2] = 0x01;
    query[3] = 0x00;   // Standard query

    query[4] = 0x00;
    query[5] = 0x01;   // One question

    /* "example.com" */
    query[pos++] = 7;
    memcpy(&query[pos], "example", 7);
    pos += 7;

    query[pos++] = 3;
    memcpy(&query[pos], "com", 3);
    pos += 3;

    query[pos++] = 0;

    /* QTYPE = A */
    query[pos++] = 0;
    query[pos++] = 1;

    /* QCLASS = IN */
    query[pos++] = 0;
    query[pos++] = 1;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in dns = {0};

    dns.sin_family = AF_INET;
    dns.sin_port = htons(53);

    inet_pton(
        AF_INET,
        "8.8.8.8",
        &dns.sin_addr
    );

    sendto(
        sock,
        query,
        pos,
        0,
        (struct sockaddr *)&dns,
        sizeof(dns)
    );

    int n = recvfrom(
        sock,
        response,
        sizeof(response),
        0,
        NULL,
        NULL
    );

    printf("Received %d bytes\n", n);

    close(sock);

    return 0;
}
