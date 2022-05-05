//azl0007 -> Austin Z Leath
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>

#define MAXLINE 4096

void echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen);

int main(int argc, char **argv)
{
    if (argc != 2) // check proper format
    {
        printf("usage: ./a.out <port>\n");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // create the socket
    if (sockfd == 0)
    {
        perror("socket failed\n");
        exit(EXIT_FAILURE); // exit with failure
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) // bind socket to address and port number
    {
        perror("bind failure\n");
        exit(EXIT_FAILURE); //exit with failure
    }

    echo(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)); //call echo
}

void echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
    int n;
    socklen_t len;
    char mesg[] = "PONG\n"; //reply with pong

    printf("[server] : ready to accept data...\n");
    srand(time(0)); // seed

    while (1) //infinite while loop
    {
        len = clilen;
        n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len); //recvfrom function

        printf("[client]: %s", mesg);

        if ((rand() % 10) < 3) //simulate packet drop
        {
            printf("[server] : packet dropped\n"); // print packet drop
        }
        else
        {
            sendto(sockfd, mesg, n, 0, pcliaddr, len); //send a response
        }
    }
}