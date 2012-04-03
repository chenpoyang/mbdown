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

void download(const char *url);
void append_bytes(FILE *file, const char *buf, const int size);
void get_data(const int fd, char *buf, const char *req);

int main(int argc, char *argv[])
{
	if (root_url == NULL)
	{
		mydebug("root_url == %s", "NULL");
	}
	if (argc >= 2 && argv[1] != NULL)
	{
		download(argv[1]);
	}
	else
	{
		merr_msg("usage: filename url");
	}

	return 0;
}

void download(const char *url)
{
	struct sockaddr_in serv;
	int sock_fd, rec_bytes;
	char hostname[64];
	char buf[4096], head_str[1024];
	FILE *file = NULL;
int sum = 0, head_len = 0, total_bytes = 0;
char *ptr = NULL, *tmp_ptr = NULL;

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
mydebug("rec_bytes %d\r\n, data: %s", rec_bytes, buf);
sum = rec_bytes;
	ptr = strstr(buf, "\r\n\r\n");
	ptr += 8;
	strncpy(head_str, buf, ptr - buf);
	head_str[ptr - buf] = '\0';
	m_tolower(head_str);
	tmp_ptr = strstr(head_str, "content-length");
	if (tmp_ptr != NULL)
	{
		sscanf(tmp_ptr, "%*[^0-9]%d", &total_bytes);
	}
	else
	{
		total_bytes = 0;
		merr_msg("content-length:0!");
	}
mydebug("total_bytes:%d bytes", total_bytes);

	sum = 0;
	if (rec_bytes - (ptr - buf) > total_bytes)
	{
		append_bytes(file, ptr, total_bytes);
		total_bytes = 0;
	}
	else
	{
		append_bytes(file, ptr, rec_bytes - (ptr - buf));
		total_bytes -= (rec_bytes - (ptr - buf));
	}
	fflush(file);
	fclose(file);
mydebug("req = %d, sizeof(buf) - (ptr - buf): %d - %d = %d", sum, sizeof(buf), (ptr - buf), sizeof(buf) - (ptr - buf));

	while (total_bytes > 0)
	{
		rec_bytes = recv(sock_fd, buf, sizeof(buf), 0);
		sum += rec_bytes;
mydebug("rec bytes = %d", rec_bytes);
		file = fopen("index.html", "a+");
		if (rec_bytes <= total_bytes)
		{
			append_bytes(file, buf, rec_bytes);
			total_bytes -= rec_bytes;
		}
		else
		{
			append_bytes(file, buf, total_bytes);
			total_bytes = 0;
		}
		fflush(file);
		fclose(file);
		if (0 == total_bytes)
		{
			break;
		}
	}

mydebug("共收到 %d 字节的数据! head_len = %d", sum, head_len);
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
