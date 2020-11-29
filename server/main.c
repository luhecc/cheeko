#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 80
#define SIZE 1024

int create_socket()
{
    int sockfd = 0;
    int ret = 0;
    struct sockaddr_in addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket create failed\n");
    }

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port   = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (-1 == bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)))
    {
        printf("bind socket failed\n");
    }

    if (-1 == listen(sockfd, 5))
    {
        printf("listen error\n");
    }

    return sockfd;
}

int wait_connect(int sockfd)
{
    struct sockaddr_in c_addr;
    int addrlen = sizeof(c_addr);
    int c_sockfd = 0;

    printf("wait connect......\n");

    if ((c_sockfd = accept(sockfd, (struct sockaddr *)&c_addr, &addrlen)) == -1)
    {
        printf("accept failed\n");
    }

    printf("one client connect, ip: %s\n", inet_ntoa(c_addr.sin_addr));

    return c_sockfd;
}

void work(int sockfd, int c_sockfd)
{
    char buf[SIZE];
    int ret = 0;
    int i = 0;

    while(1)
    {
        ret = read(c_sockfd, buf, SIZE - 1);

        if (-1 == ret)
        {
            printf("read failed\n");
        }
        else if(0 == ret)
        {
            break;
        }

        buf[ret] = '\0';

        for(i; i < ret; i++)
        {
            buf[i] = buf[i] + 'A' - 'a';
        }

        printf("%s\n", buf);

        write(c_sockfd, buf, ret);

        if (!strncmp(buf, "end", 3))
        {
            break;
        }
    }

    close(c_sockfd);
}

int main()
{
    int sockfd = 0;
    int c_sockfd = 0;

    sockfd = create_socket();
    c_sockfd = wait_connect(sockfd);

    work(sockfd, c_sockfd);

    close(sockfd);

    return 0;
}
