#include "lib.h"

int tcp_connect(const char *host, const char *serv)
{
	int		socket_desc, n;
	struct addrinfo hints, *res, *ressave;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((n = getaddrinfo(host, serv, &hints, &res)) != 0)
	{
		printf("getaddrinfo tcp_connect: %s\n", gai_strerror(n));
		exit(EXIT_FAILURE);
	}

	ressave = res;

	do
	{
		socket_desc = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(socket_desc < 0)
			continue;

		if(connect(socket_desc, res->ai_addr, res->ai_addrlen) == 0)
			break;

		checkedClose(socket_desc);
	} while(res = res->ai_next != NULL);

	if(res == NULL)
	{
		printf("tcp_connect error\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(ressave);

	return(socket_desc);
}
