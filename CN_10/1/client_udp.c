#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/un.h>
#include<signal.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#define MAX 256
int
main ()
{
  int sfd = socket (AF_INET, SOCK_DGRAM, 0);
  if (sfd == -1)
    printf ("sock error\n");
  struct sockaddr_in *myaddr, clientaddr;
  int length = sizeof (struct sockaddr_in);
  char x[MAX];
  myaddr = malloc (sizeof (struct sockaddr_in));
  myaddr->sin_family = AF_INET;
  myaddr->sin_port = htons (2367);
  myaddr->sin_addr.s_addr = INADDR_ANY;
  int b = bind (sfd, (struct sockaddr *) myaddr, sizeof (struct sockaddr_in));
  if (b < 0)
    exit (0);

  while (1)
    {
      memset (x, 0, MAX);
      recvfrom (sfd, x, MAX, 0, (struct sockaddr *)& clientaddr, &length);
      printf ("%s", x);
      printf ("%d\n", htons (clientaddr.sin_port));
      sendto (sfd, x, MAX, 0, (struct sockaddr *)& clientaddr, length);
    }
  return 0;
}
