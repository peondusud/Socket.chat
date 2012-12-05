#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <string.h>
#include "libnet.h"
#include <time.h>

/*
@param
	arg1: port
	default port = 2222

To compile it:
gcc -c libnet.c
gcc -o serv peon_serv.c libnet.o

To run it:
./serv
*/

void main(int argc, char **argv)
{
    time_t secondes;
    struct tm instant;
    int fd;
    struct	in_addr ip_resolv[1000]= {0,0,get_host_ip("0.0.0.0"),0};
    FILE * f;
    struct sockaddr_in listening_addr;
    short port_number = 2222;
    int fdmax=10;
    int r,i,j,len;
    char buffer[100]= {0};
    struct sockaddr_in client_addr;
    int client_addr_len;
    int client_fd;
    fd_set in_set, master;

    if(argc = 2)
    {
        port_number=atoi(argv[1]);
    }

    f = fopen ( "peonchat.log" , "a" );
    /* The server creates the socket same way as the client */
    fd = socket(AF_INET, SOCK_STREAM, 0);
    char str[256];

    if (fd < 0)
    {
        fprintf(stderr, "socket() failed\n");
        return;
    }

    /* prepare the socket address for binding */
    listening_addr.sin_family = AF_INET;
    listening_addr.sin_port = htons(port_number);
    listening_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* bind the socket */
    if (bind(fd, (struct sockaddr *)&listening_addr, sizeof(listening_addr))==-1)
    {
        fprintf(stderr, "bind() failed\n");
        return;
    }
    fprintf (f, "socket %d successfully bound\n", fd);
    printf("socket %d successfully bound\n", fd);

    r = listen(fd, fdmax);

    if (r < 0)
    {
        fprintf (f,"socket %d successfully bound\n", fd);
        fprintf(stderr, "listen() failed\n");
        return;
    }
    fprintf (f,"listen successfull\n");
    printf("listen successfull\n");
    FD_ZERO(&master);
    FD_ZERO(&in_set);

    // add fd to in_set
    //FD_SET(fd, &in_set);
    FD_SET(fd, &master);

    //trick
    fdmax = fd;

    while (1)
    {

        // copy it
        in_set = master;

        //new data??
        if ((select(fdmax+1, &in_set, NULL, NULL, NULL))==-1)
        {
            fprintf(stderr, "select() failed\n");
            return;
        }
        for(i=0; i<=fdmax; i++)
        {
            //Who?
            if ( FD_ISSET(i,&in_set) )
            {
                //new connection
                if(i==fd)
                {
                    client_addr_len = sizeof(client_addr);
                    client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_addr_len);
                    fprintf (f,"successfull connection from %s, file descriptor is %d\n",
                             inet_ntoa(client_addr.sin_addr), client_fd);
                    printf("successfull connection from %s, file descriptor is %d\n",
                           inet_ntoa(client_addr.sin_addr), client_fd);
                    if (client_fd < 0)
                    {
                        fprintf(stderr, "accept() failed\n");
                        return;
                    }

                    // update  maximum FD
                    if(client_fd > fdmax)
                        fdmax=client_fd;
                    //add ip_resolv[fd number]=IP
                    ip_resolv[client_fd]=client_addr.sin_addr;
                    FD_SET(client_fd,&master);
                }
                else
                {
                    len=read(i, buffer, sizeof(buffer));
                    if (len < 0)
                    {
                        close(i);
                        FD_CLR(i,&master);
                        fprintf(stderr, "read() failed\n");
                        return;
                    }
                    buffer[len]=0;
                    //close conn
                    if (len == 0)
                    {
                        close(i);
                        FD_CLR(i,&master);
                        fprintf(f,"%d/%d/%d - %02d:%02d:%02d * ",instant.tm_mday,
                                instant.tm_mon+1,
                                instant.tm_year+1900,
                                instant.tm_hour,
                                instant.tm_min,
                                instant.tm_sec);
                        fprintf (f,"successfull connection from %s, file descriptor is %d\n",
                                 inet_ntoa(client_addr.sin_addr), client_fd);


                    }
                    //read data available
                    if (len > 0)
                    {

                        time(&secondes);
                        instant=*localtime(&secondes);
                        sprintf(str,"%d/%d/%d - %02d:%02d:%02d @%s >> %s",instant.tm_mday,
                                instant.tm_mon+1,
                                instant.tm_year+1900,
                                instant.tm_hour,
                                instant.tm_min,
                                instant.tm_sec,
                                inet_ntoa(ip_resolv[i]),
                                buffer);






                        len = strlen(str);

                        for(j=0; j<=fdmax; j++)
                        {
                            if ( FD_ISSET(j,&master) )
                            {
                                if(j!=fd)
                                {
                                    //write in all client
                                    if (write(j, str, len) < 0)
                                    {
                                        fprintf(stderr, "write() failed\n");
                                        return;
                                    }

                                }

                            }
                        }
                        //echo in serv
                        printf ("%s",str);
                        //in server log
                        fprintf (f,"%s",str);
                        fflush(f);
                    }
                }
            }
        }
    }
    fclose(f);
}
