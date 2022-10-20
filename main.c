
// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>

#define PORT     5000
#define MAXLINE 1024

// Driver code
int main(int argc, char** argv) {

    int xM = 0;
    for(int i = 0; i < argc; i++)
    {
        if(strstr(argv[i], "-xM"))
            xM = 1;
    }

    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int len, n;

    len = sizeof(cliaddr);  //len is value/resuslt
    int run = 1;

    while(run){
        printf("waiting for command... (PORT : %d)\n", PORT);
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                    (socklen_t*)&len);
        buffer[n] = '\0';
        char ip4_ad[INET_ADDRSTRLEN];
        inet_ntop(cliaddr.sin_family, &cliaddr.sin_addr, ip4_ad, sizeof ip4_ad);


        printf("Received FROM <%s>:\n%s\n\n",ip4_ad,buffer);
        if(buffer[0] == 'M' && xM)
        {
            int cmd_sz = strlen(buffer) - 1;
            char cmd[cmd_sz];
            cmd[cmd_sz] = '\0';

            for(int i = 0; i < cmd_sz; i++)
            {
                cmd[i] = tolower(buffer[i+1]);
            }

            if(!strcmp(cmd,"quit") || !strcmp(cmd,"exit"))
            {
                run = 0;
            }else{
                printf("Parsed and Executing : %s\n\n",cmd);
                system(cmd);
            }
        }

        printf("\n");

    }

    printf("exiting...\n");
    return 0;
}
