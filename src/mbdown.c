#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "global.h"

void download(const char *url);
void bzero(void *base, const u_int size); 
void get_host_name(const char *url, char *hostname);

int main(int argc, char *argv[])
{
	if (argc >= 2 && argv[1] != NULL)
	{
		download(argv[1]);
	}
	else
	{
		m_error("usage: filename url");
	}

	return 0;
}

void download(const char *url)
{
	struct sockaddr_in serv;
	int sock_fd;
	char hostname[64];
	char buf[4096];

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		m_error("socket error!");
		return;
	}

	/* 初始化服务器socket地址 */
	bzero(&serv, sizeof(serv));
	get_host_name(url, hostname);
	serv.sin_family = AF_INET;
	serv.sin_port = htons(80);

	if (inet_pton(AF_INET, hostname, &serv.sin_addr) < 0)
	{
		m_error("inet_pton error!");
		return;
	}

	if (connect(sock_fd, (struct sockaddr *)&serv, sizeof(serv)) < 0)
	{
		m_error("connect error!");
		return;
	}

	snprintf(buf, sizeof(buf), 
			"GET /index.html HTTP/1.0\r\nHost:%s\r\n\r\n", 
			hostname);

	send(sock_fd, buf, sizeof(buf), 0);
	recv(sock_fd, buf, sizeof(buf), 0);
	puts(buf);
}

void get_host_name(const char *url, char *hostname)
{
	char *ptr = NULL, *str_ptr = NULL, *str = NULL;
	int len;

	assert(url != NULL && hostname != NULL);

	len = strlen(url);
	str = (char *) calloc (1, sizeof(len));
	assert(str != NULL);

	strcpy(str, url);
	m_tolower(str);
	str_ptr = strstr(str, "http://");
	if (str_ptr != NULL)
	{
		str_ptr += 7;
	}
	else
	{
		str_ptr = str;
	}

	ptr = strstr(str_ptr, "/");

	if (ptr != NULL)
	{
		len = ptr - str_ptr;
	}
	else
	{
		len = strlen(str_ptr);
	}
	
	strncpy(hostname, str_ptr, len);
	hostname[len] = '\0';

	free(str);
}

void bzero(void *base, const u_int size)
{
	memset(base, 0, size);
}
