#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#define msg "Sending data"
#define MAXSIZE 514

int main(int argc, char ** argv)
{
	if (argc!=4)
	{
		printf("Wrong usage.\nPlease use %s hostname port file",argv[0]);
		exit(EXIT_FAILURE);
	}
	printf("Welcome to 2G2TD1TP1's imlementation of a tftp client\n");
	
	
	struct addrinfo * result;
	struct addrinfo hints;
	struct sockaddr portserv_addr;
	socklen_t portserv_addr_len;
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family=AF_INET;
	hints.ai_protocol=IPPROTO_UDP;
	hints.ai_socktype=SOCK_DGRAM;
	int status;
	status = getaddrinfo(argv[1],argv[2],&hints,&result);
	char buffer[MAXSIZE]={0};
	buffer[1]=1;
	sprintf(buffer+2,"%s","file.txt");
	sprintf(buffer +3+strlen("file.txt"),"NETASCII");
	char rcvbuffer[MAXSIZE]={0};
	char rcvDatabuffer[MAXSIZE]={0};
	int sock = socket(hints.ai_family,hints.ai_socktype,hints.ai_protocol);
	
	// Vérification
	
	if (status==-1)
	{
		printf("It seems like I can't find the host %s\n",argv[1]);
	}
	if (sock==-1)
	{
		perror("socket error");
		exit(EXIT_FAILURE);
	}
	
	// Envoi d'un fichier file.txt
	
	sendto(sock,buffer,12+sizeof(argv[3]),0,result->ai_addr,result->ai_addrlen);
	printf("Receivedps -a %s to the host : %s@%s\n", argv[3],argv[1],argv[2]);
	
	// Réception d'un accusé de réception (Acknowledge)
	
	int nbByteReceived = recvfrom(sock,rcvbuffer,sizeof(rcvbuffer),MSG_WAITALL,&portserv_addr,&portserv_addr_len);
	
	// Vérification d'une potentielle erreur dans la transmission grâce au 2e octet du acknowledge
	
	switch(rcvbuffer[1])
	{
		case 5 :
			printf("Error from the server : code %d :\n%s\n",rcvbuffer[3],rcvbuffer+4);
			exit(EXIT_SUCCESS);
	}
	printf("Acknowledge size : %d\n",nbByteReceived);
	
	// Envoi de data sur le port de retour du serveur

	sendto(sock,msg,12+strlen(msg),0,&portserv_addr,&portserv_addr_len);
	int nbByteReceived2 = recvfrom(sock,rcvDatabuffer,sizeof(rcvDatabuffer),MSG_WAITALL,&portserv_addr,&portserv_addr_len);
	switch(rcvDatabuffer[1])
	{
		case 5 :
			printf("Error from the server : code %d :\n%s\n",rcvDatabuffer[3],rcvDatabuffer+4);
			exit(EXIT_SUCCESS);
	}
	printf("Acknowledge size : %d\n",nbByteReceived2);
	
	return EXIT_SUCCESS;
}
