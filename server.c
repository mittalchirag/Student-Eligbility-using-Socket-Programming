#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <my_global.h>
#include <mysql.h>

#define MAX 12
#define PORT 8080
#define BACKLOG 10
#define SA struct sockaddr

typedef struct student
{
    char value[2];
} student;

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}
void cpy(char *dest, char *src, int n)
{
    for (int i = 0; i < n; i++)
    {
        dest[i] = src[i];
    }
    dest[n] = '\0';
}
void fetch_data(char *RNo, student *Criteria)
{
    char query[100];
    strcpy(query, "SELECT * FROM eligibility_list where Reg_No='");
    strcat(query, RNo);
    strcat(query, "'");

    int len = strlen(query);

    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "chirag", "chirag123", "STUDENTS", 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 1; i < num_fields; i++)
        {
            memset(Criteria[i - 1].value, '\0', sizeof(Criteria[i - 1].value));
            strcpy(Criteria[i - 1].value, row[i]);
        }
    }

    mysql_free_result(result);
    mysql_close(con);
}


// Function designed for chat between client and server.
void func(int sockfd)
{
    char registration_number[MAX];

    for (;;)
    {
        bzero(registration_number, MAX);
        read(sockfd, registration_number, sizeof(registration_number));
        printf("Registration Number queried: %s\n ", registration_number);

        if ((strncmp(registration_number, "EXIT", 4)) == 0)
        {
            printf("Server Exit...\n");
            close(sockfd);
            exit(0);
        }

        student Criteria[6];
        fetch_data(registration_number, Criteria);

        printf("Data send to client: ");
        for (int i = 1; i < 7; i++)
        {
            char BUF[2];
            memset(BUF, '\0', sizeof(BUF));
            cpy(BUF, Criteria[i - 1].value, 2);
            printf("%s\t", BUF);
            send(sockfd, BUF, 2, 0);
        }
        printf("\n");
    }
}

// Driver function
int main()
{
    int sockfd, connfd, len; //socket file 
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        close(sockfd);
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        close(sockfd);
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, BACKLOG)) != 0)
    {
        printf("Listen failed...\n");
        close(sockfd);
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("server acccept failed...\n");
        close(sockfd);
        exit(0);
    }
    else
        printf("server acccepted the client...\n");

    func(connfd);

    close(sockfd);
}