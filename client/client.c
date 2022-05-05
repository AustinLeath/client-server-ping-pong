//azl0007 -> Austin Z Leath
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>

void cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen);

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 3) // incorrect command line format
    {
        printf("usage : ./a.out <hostname> <port>\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    // get ip from host name
    struct hostent *hosten;
    hosten = gethostbyname(argv[1]);
    if (hosten == NULL) // ip corresponds with name
    {
        printf("hostname not found\n");
        exit(EXIT_FAILURE); // exit with failure
    }

    memcpy(&servaddr.sin_addr, hosten->h_addr_list[0], hosten->h_length); // take in ip to memcpy
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // create socket for use in ops
    if (sockfd == 0)
    {
        perror("socket failed\n");
        exit(EXIT_FAILURE); //exit with failure
    }

    //create timeout
    struct timeval tv;
    tv.tv_sec = 1; //set to 1
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) //here we set a sock op
    {
        perror("setsockopt error\n");
        exit(EXIT_FAILURE); //exit with failure
    }

    cli(stdin, sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    exit(0); //exit with 0
}

void cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen) // cli function
{
    int n, i;
    char sendline[] = "PING\n", recvline[21];
    suseconds_t min = 9000000, max = 0, avg = 0;
    double sent = 0, received = 0;

    for (i = 0; i < 10; i++) //ping 10 times
    {
        struct timeval begin, end;                                         // begin and end
        gettimeofday(&begin, NULL);                                        // start time of day
        sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen); // sendto used for ping function
        sent++;
        n = recvfrom(sockfd, recvline, 20, 0, NULL, NULL); // get response
        gettimeofday(&end, NULL);                          // end all
        recvline[n] = '\0';                                //terminate

        printf("%2d : sent PING... ", i + 1);
        struct timeval diff; // store RTT for later
        diff.tv_sec = 0;
        diff.tv_usec = (end.tv_sec - begin.tv_sec) * 1000000 + end.tv_usec - begin.tv_usec;
        suseconds_t usec = diff.tv_usec;
        if (n >= 0)
        {
            // not lost
            printf("received b'PONG'\n");
            received++;
            min = (usec < min) ? usec : min;
            max = (usec > max) ? usec : max;
            avg = avg + usec;
        }
        else
        {
            printf("Timed out\n"); // lost due to timeout
        }
    }
    double average = (double)avg / received;
}