#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#define SERVER_PORT 8888

int main(int argc, char **argv)
{
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;

	int iRet;
	unsigned char ucSendBuf[1000];
	int iSendLen;
	int iAddrLen;

	if (argc != 2)
	{
		printf("Usage: \n");
		printf("%s <server_ip>\n", argv[0]);
		return -1;
	}

	iSocketClient = socket(AF_INET, SOCK_DGRAM, 0);

	tSocketServerAddr.sin_family = AF_INET;
	tSocketServerAddr.sin_port = htons(SERVER_PORT);

 	if (0 == inet_aton(argv[1], &tSocketServerAddr.sin_addr))
	{
		printf("invalid server_ip");
		return -1;
	}
	memset(tSocketServerAddr.sin_zero, 0, 8);

	while (1)
	{
		if (fgets(ucSendBuf, 999, stdin))
		{
			iAddrLen = sizeof(struct sockaddr);
			iSendLen = sendto(iSocketClient, ucSendBuf, strlen(ucSendBuf), 0, 
								(const struct sockaddr *)&tSocketServerAddr, iAddrLen);
			if (iSendLen <= 0)
			{
				close(iSocketClient);
				return -1;
			}
		}
	}
	return 0;
}