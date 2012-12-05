#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>

/*
  A utility function.
  Takes one argument: the host name
  Returns: the IP address, or 0 if not found
*/

in_addr_t get_host_ip(char *hostname)
{
  struct hostent *resolved;
  struct in_addr *ipaddr;

  /* query the DNS */
  resolved = gethostbyname(hostname);

  /* if returned value is NULL, host is not known. We give up. */
  if (resolved == NULL) {
    fprintf( stderr, "Error: host %s is not known.\n", hostname);
    return 0;
  }

  /* Here, host is known. Let dig into the structures to find its 
     IP adress. It is the first element of the array h_addr_list
     in the struct hostent.
     We must cast it (i.e. convert the pointers) because it is declared
     as an array of char *
  */
  ipaddr = (struct in_addr *)resolved->h_addr_list[0];

  return ipaddr->s_addr;
}
