#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAX 12
#define PORT 8080
#define SA struct sockaddr

int computeEligibility(int tt1, int tt2, int tt3, int attendance, int a1, int a2){
    int b1=0,b2=0;
    if(tt1>=tt2){
        b1=tt1;
        b2= (tt2>= tt3) ? tt2 : tt3;
    }
    else if (tt1<tt2){
        b1=tt2;
        b2= (tt1>= tt3) ? tt1 : tt3;
    }

    int total = (a1+a2+b1+b2);

    if (total < 40)
        return 1;
    if(attendance < 80)
        return 2;

    return 0;
}

void cpy(char *dest, char *src, int n){
    for(int i = 0; i < n; i++)
    {
        dest[i]=src[i];
    }
    dest[n]='\0';
    
}
void func(int sockfd)
{
    char registration_number[MAX];
    
    int n;
    for (;;)
    {
        bzero(registration_number, sizeof(registration_number));
        printf("\nEnter the registration number : ");
        n = 0;
        while ((registration_number[n++] = toupper(getchar())) != '\n');
        cpy(registration_number,registration_number,12);
        if ((strncmp(registration_number, "EXIT", 4)) == 0)
        {
            printf("Client Exit...\n");
            close(sockfd);
            exit(0);
        }
        write(sockfd, registration_number, sizeof(registration_number));

        char studentCriteria[6][2];
        int studentCriteriaInt[6];
        memset(studentCriteria, '\0', sizeof(studentCriteria));
        printf("\n========================== TT1\tTT2\tTT3\tAttd.\tA1\tA2\n");
        printf("Data Received from server: ");
        for (int i = 0; i < 6; i++)
        {
            char buffer[2];
            recv(sockfd, buffer, 2, 0);
            cpy(studentCriteria[i],buffer,2);
            studentCriteriaInt[i]=atoi(studentCriteria[i]);
            printf("%d\t", studentCriteriaInt[i]);
        }
        printf("\n\n");
        int res=computeEligibility(studentCriteriaInt[0],studentCriteriaInt[1],studentCriteriaInt[2],studentCriteriaInt[3],studentCriteriaInt[4],studentCriteriaInt[5]);
        if(res==0){
            printf("%s is Eligible to give SEE!!!\n", registration_number);
        }else if(res==1){
            printf("Sorry! %s is non-eligible to give SEE due to less Marks in component 1\n",registration_number);
        }else if(res==2){
            printf("Sorry! %s is non-eligible to give SEE due to low attendance\n",registration_number);
        }
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for sneding and recieving data
    func(sockfd);

    // close the socket
    close(sockfd);
}