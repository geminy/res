// server
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
    int clifd;
    char srcletter;
    char desletter;
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("socket error: %s\n", strerror(errno));
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SOCKPORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY native ip

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        printf("bind error: %s\n",strerror(errno));
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, 1) == -1) {
        printf("listen error: %s\n", strerror(errno));
        close(sockfd);
        return -1;
    }
    printf("===== waiting for client's request =====\n");

    if((clifd = accept(sockfd, (struct sockaddr*)NULL, NULL)) == -1) {
        printf("accept error: %s\n", strerror(errno));
        close(sockfd);
        return -1;
    }

    while (1) {
        if (recv(clifd, &srcletter, sizeof(srcletter), 0) == -1) {
            printf("recv error: %s\n", strerror(errno));
            break;
        }

        if (isupper(srcletter)) {
            desletter = tolower(srcletter);
        }
        else if (islower(srcletter)) {
            desletter = toupper(srcletter);
        }
        printf("recv [%c] send[%c]\n", srcletter, desletter);

        if (send(clifd, &desletter, sizeof(desletter), 0) == -1) {
            printf("send error: %s\n", strerror(errno));
            break;
        }
    }

    close(clifd);
    close(sockfd);
    return 0;
}
