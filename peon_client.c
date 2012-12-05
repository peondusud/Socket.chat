#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdio.h>
#include <string.h>
#include "libnet.h"
#include <stdlib.h>
#include "splitscreen.h"
#include <unistd.h>

#define OUTPUT_WINDOW_LINES 20
#define N 128
/*
@param 	arg1: hostname
	arg2: port
	default hostname = localhost
	default port = 2222

To compile it:
gcc -c libnet.c
gcc -c splitscreen.c
gcc -o client peon_client.c *.o -lcurses
To run it:
./client
*/

/*
  A local utility function.
  Creates a client socket, connected to hostname, port port_number
  Returns: the socket, or -1 if error
*/
int create_client_socket(char *hostname, short port_number)
{
    in_addr_t host_ipaddr;
    int fd, r;
    struct sockaddr_in server_addr;

    /* Let's create the socket */
    fd = socket( AF_INET, SOCK_STREAM, 0);

    /* Test if socket creation failed */
    if (fd < 0)
    {
        fprintf( stderr, "socket() failed\n");
        return -1;
    }

    /* socket is ok, we can try to connect. */
    /* first, we ask the DNS for our host IP */
    host_ipaddr = get_host_ip( hostname);

    /* Check the result: if 0, host is unknown */
    if (host_ipaddr == 0)
    {
        fprintf( stderr, "host unknown\n");
        return -1;
    }

    /* We have our host IP, we can fill in the socket address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( port_number); /* Remember: in network byte order!!! */
    server_addr.sin_addr.s_addr = host_ipaddr;

    /* and we connect */
    r = connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    /* check the result */
    if (r < 0)
    {
        fprintf( stderr, "connect() failed\n");
        return -1;
    }

    return fd;
}

/*
   Read standard input and write to the server.
   Returns -1 in case of error, 0 if end of input,
   1 otherwise.
*/
int process_stdin(int fd)
{
    char buffer[100]= {0};
    int len;


    splitscreen_input_line( buffer, sizeof(buffer));
    /* write the read buffer */
    len = write(fd, buffer, strlen(buffer));

    /* check return value */
    if (len < 0)
    {
        fprintf(stderr, "write() failed\n");
        return -1;
    }

    return 1;
}

/*
   Reads socket and write to standard output.
   Returns -1 in case of error, 0 if end of input,
   1 otherwise.
*/
int process_server(int fd)
{
    char buffer[100]= {0};
    int len;

    /* read back from server */
    len = read(fd, buffer, sizeof(buffer));

    if (len < 0)
    {
        fprintf(stderr, "read() failed\n");
        return -1;
    }

    if (len == 0)
        return 0;
    splitscreen_output( buffer);
    // write(1, buffer, len);

    return 1;
}

main(int argc, char **argv)
{
    char line[N];
    char *hostname = "localhost";
    short port_number = 2222;
    int fd,c;
    /*  char *pvalue = NULL;

            while ((c = getopt (argc, argv, "a:ph:")) != -1)
           switch (c)
             {
             case 'a':
               hostname = optarg;
               break;
             case 'p':

              port_number = atoi(optarg);
               //printf("%d",port_number);
               break;
             case 'h':
              // help();
               break;
             case '?':
               if (optopt == 'a' || optopt == 'p' )
                 fprintf (stderr, "Option -%c requires an argument.\n", optopt);
               else if (isprint (optopt))
                 fprintf (stderr, "Unknown option `-%c'.\n", optopt);
               else
                 fprintf (stderr,
                          "Unknown option character `\\x%x'.\n",
                          optopt);
               return 1;
             default:
               break;
             }

      */
    if(argc = 3)
    {
        hostname=argv[1];
        port_number=atoi(argv[2]);

    }

    splitscreen_init( OUTPUT_WINDOW_LINES, NULL);

    splitscreen_output( "Peon CHat");

    /* create the client socket using utility function */
    fd = create_client_socket(hostname, port_number);

    /* check return value */
    if (fd < 0)
    {
        fprintf(stderr, "create_client_socket() failed\n");
        return;
    }

    /* loop reading standard input */
    while (1)
    {

        fd_set readfs;
        int r;

        FD_ZERO(&readfs);
        FD_SET(0,&readfs);
        FD_SET(fd,&readfs);

        r = select(fd+1, &readfs, NULL, NULL, NULL);

        if (r < 0)
        {
            fprintf(stderr, "select() failed\n");
            return;
        }

        if (FD_ISSET(fd, &readfs))
        {
            r = process_server(fd);
            if (r <= 0)
                return;
        }
        splitscreen_prompt();
        if (FD_ISSET(0, &readfs))
        {
            r = process_stdin(fd);
            if (r <= 0)
                return;
        }

    }
    splitscreen_quit();
}
