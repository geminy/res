// client
// character convertion between upper and lower

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SOCKPORT 8000

int main(int argc, char** argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    char sendletter;
    char recvletter;

    if (argc != 2) {
        printf("usage: ./client <server_ip>\n");
        return -1;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket error: %s\n", strerror(errno));
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SOCKPORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        printf("inet_pton error: %s, ip is %s\n", strerror(errno), argv[1]);
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        printf("connect error: %s\n", strerror(errno));
        close(sockfd);
        return -1;
    }

    while (1) {
        printf("\ninput a character:\n\talphabet - convert between upper and lower\n\tdigit 0 - quit\n");
        printf("\n\tyour characer: ");
        scanf("%c", &sendletter);
		getchar();

        if (strncmp(&sendletter, "0", 1) == 0) {
            printf("quit\n");
            break;
        }

        if (send(sockfd, &sendletter, sizeof(sendletter), 0) == -1) {
            printf("send error: %s\n", strerror(errno));
            break;
        }

        if (recv(sockfd, &recvletter, sizeof(recvletter), 0) == -1) {
            printf("recv error: %s\n", strerror(errno));
            break;
        }

        printf("\tfrom [%c] to [%c]\n", sendletter, recvletter);
    }

    close(sockfd);
    return 0;
}
