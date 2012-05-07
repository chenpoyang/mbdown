#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, const char *argv[])
{
	struct hostent *addr = NULL;
	char **ptr_alias = NULL;
	char ip_buf[16];

	if (argc != 2)
	{
		printf("usage: %s domain\n", argv[1]);
		exit(-1);
	}

	addr = gethostbyname(argv[1]);
	if (addr == NULL)
	{
		printf("illegal domain name!");
		exit(-1);
	}

	printf("global name:%s\n\t", addr->h_name);
	ptr_alias = addr->h_aliases;
	while (*ptr_alias != NULL)
	{
		printf("alias: %s\n", *ptr_alias);
		++ptr_alias;
	}

	ptr_alias = addr->h_addr_list;
	while (*ptr_alias != NULL)
	{
		printf("ip: %s\n", inet_ntop(addr->h_addrtype, *ptr_alias, ip_buf, sizeof(ip_buf)));
		++ptr_alias;
	}

	return 0;
}
