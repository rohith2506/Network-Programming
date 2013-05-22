#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>
#include <signal.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

int main()
{
int sfd=socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
if(sfd<0)
{
printf("socket error\n");
exit(0);
}
char buf[2048]={0};

while(1)
{
struct ip* ipdr=buf;
struct sockaddr_in cliaddr;
socklen_t len;
if(recvfrom(sfd,buf,4096,0,(struct sockaddr *)& cliaddr,&len)<0)
{
printf("recv error\n");
exit(0);
}
printf("\t\t\t#########################################\t\t\t\n");
printf("\t\t\t#\tIP header len:\t%d\t\t#\n",ipdr->ip_hl*4);
printf("\t\t\t#\tIP version is:\t%d\t\t#\n",ipdr->ip_v);
printf("\t\t\t#\tIP service   :\t%d\t\t#\n",ipdr->ip_tos);
printf("\t\t\t#\tIP len         :\t%d\t#\n",ipdr->ip_len);
printf("\t\t\t#\tIP sequence id :\t%d\t#\n",ipdr->ip_id);
printf("\t\t\t#\tIP frag offset :\t%d\t#\n",ipdr->ip_off);
printf("\t\t\t#\tIP TTL         :\t%d\t#\n",ipdr->ip_ttl);
printf("\t\t\t#\tHigher level prtcol:\t%u\t#\n",ipdr->ip_p);
printf("\t\t\t#\tCheckSum       :\t%x\t#\n",ipdr->ip_sum);
printf("\t\t\t#\tSRC addr       :%s\t#\n",inet_ntoa(ipdr->ip_src));
printf("\t\t\t#\tDST addr       :%s\t#\n",inet_ntoa(ipdr->ip_dst));
printf("\t\t\t#########################################\t\t\t\n");
printf("\n");
}
return 0;
}
