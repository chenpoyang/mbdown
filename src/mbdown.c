#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "global.h"
#include "url.h"

GLOBAL RootUrl *root_url;				/* root url manager */
GLOBAL unsigned int count_url_id;		/* for generating unique url id */
GLOBAL HaUrl url_htable[HALEN];			/* hash table */

void download(const Url *url);
void get(const char *url);
void append_bytes(FILE *file, const char *buf, const int size);
void get_data(const int fd, char *buf, const char *req);

int main(int argc, char *argv[])
{
	Url *url = NULL;
	int len;

	/**
	 * @brief	初始化 url 哈希表和 url 管理器
	 */
	len = HALEN;
	root_url = NULL;
	init_hatable_rooturl_id(&root_url, url_htable, len, &count_url_id);
	url = new_url_node(&root_url, argv[1], url_htable, len);

	if (root_url == NULL)
	{
		mydebug("root_url == %s", "NULL");
	}
	if (argc >= 2 && argv[1] != NULL)
	{
get(argv[1]);
		download(url);
	}
	else
	{
		merr_msg("usage: filename url");
	}

	release_url_all(&root_url, url_htable, HALEN);

	return 0;
}


void download(const Url *url)
{
	//unsigned int total_bytes;
	//struct sockaddr_in serv;
	//char buf[MAXBUF];

	print_url(url);
}

void get(const char *url)
{
	struct sockaddr_in serv;
	int sock_fd, rec_bytes;
	char hostname[64];
	char buf[MAXBUF];

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
			"GET /index.html HTTP/1.1\r\nHost:%s\r\n\r\n", 
			hostname);
	send(sock_fd, buf, sizeof(buf), 0);
	
	rec_bytes = recv(sock_fd, buf, sizeof(buf), 0);
}

/* 加入新的数据 */
void append_bytes(FILE *file, const char *buf, const int size)
{
	fwrite(buf, 1, size, file);
}


/* 从fd中读取收到的数据, 用buf[]保存并返回 */
void get_data(const int fd, char *buf, const char *req)
{
}
