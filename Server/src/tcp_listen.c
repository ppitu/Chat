#include "lib.h"

int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp)
{
	int 			listen_desc, n;
	const int		on = 1;
	struct addrinfo	hints, *res, *ressave;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((n = getaddrinfo(host, serv, &hints, &res)) != 0)
	{
		printf("getaddrinfo error: %s, %s: %s\n", host, serv, gai_strerror(n));
		exit(EXIT_FAILURE);
	}

	ressave = res;

	do {
		listen_desc = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(listen_desc < 0)
			continue; /*blad probujemy nastepny*/

		checkedSetsockopt(listen_desc, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));


		if(bind(listen_desc, res->ai_addr, res->ai_addrlen) == 0)
			break;

		checkedClose(listen_desc);
	} while((res = res->ai_next) != NULL);

	if(res == NULL)
	{
		printf("tcp_listen error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	checkedListen(listen_desc, 20);

	if(addrlenp)
		*addrlenp = res->ai_addrlen;

	freeaddrinfo(ressave);

	return(listen_desc);
}
