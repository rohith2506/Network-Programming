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
#include<netinet/ip.h>
#include<netinet/udp.h>

unsigned short csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}


int main()
{
int sfd=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
char buf[4096]={0};
struct ip* ip_hdr=buf;
ip_hdr->ip_hl=5;
ip_hdr->ip_v=4;
ip_hdr->ip_tos=0;
ip_hdr->ip_len=20+8+3;
ip_hdr->ip_id=10000;
ip_hdr->ip_off=0;
ip_hdr->ip_ttl=64;
ip_hdr->ip_p=17;
inet_pton(AF_INET,"127.0.0.1",&(ip_hdr->ip_src));
inet_pton(AF_INET,"127.0.0.1",&(ip_hdr->ip_dst));
ip_hdr->ip_sum=csum((unsigned short*)buf,9);

    int one = 1;
    const int *val = &one;
    if (setsockopt (sfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Cannot set HDRINCL!\n");
      
struct udphdr *ud=buf+ip_hdr->ip_hl*4;

ud->source=htons(33333);
ud->dest=htons(2367);
ud->len=htons(8+4);
ud->check=0;
memcpy(buf+20,ud,8);
memcpy(buf+28,"bye\n",4);

struct sockaddr_in addr;
addr.sin_port=htons(2367);
addr.sin_family=AF_INET;
inet_pton(AF_INET,"127.0.0.1",&(addr.sin_addr));
while(1)
{
sleep(1);
sendto(sfd,buf,32,0,(struct sockaddr *)&addr,sizeof addr);
}

return 0;
}
