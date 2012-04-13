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
void get_filename(const char *url, char *filename);

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
//
//if (root_url == NULL)
//{
//	mydebug("root_url == %s", "NULL");
//}
	if (argc >= 2 && argv[1] != NULL)
	{
		get(argv[1]);
		download(url);
	}
	else
	{
		merr_msg("usage: filename url");
	}

	//release_url_all(&root_url, url_htable, HALEN);

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
	int sock_fd, rec_bytes, is_first, totals;
	char hostname[64];
	char buf[MAXBUF], res[MAXBUF];
	char *ptr = NULL, *tmp_ptr = NULL;
	FILE *file = NULL, *file_tmp;
	char filename[64];
	char resource[1024];

	get_filename(url, filename);
	get_resource(url, resource);

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
			"GET %s HTTP/1.1\r\nUser-Agent: mbdown/1.0\r\nHost: %s\r\n\r\n", 
			resource, hostname);
	puts("send:");
	puts(buf);
	send(sock_fd, buf, sizeof(buf), 0);

	is_first = 1;
	file = fopen(filename, "wb");
	file_tmp = file;
	while ((rec_bytes = recv(sock_fd, buf, sizeof(buf), 0)) > 0)
	{
		if (is_first)
		{
			is_first = 0;
			ptr = strstr(buf, "\r\n\r\n");
			if (ptr == NULL)
			{
				merr_sys("bad receive!");
				return;
			}

			/**
			 * @brief	获取头部
			 */
			ptr += 4;
			strncpy(res, buf, ptr - buf);
			res[ptr - buf] = '\0';
			puts("res header:");
			puts(res);

			m_tolower(res);
			tmp_ptr = strstr(res, "length");
			if (NULL == tmp_ptr)
			{
				merr_sys("bad header!");
				return;
			}
			sscanf(tmp_ptr, "%*s%d", &totals);
			rec_bytes -= (ptr - buf);
			is_first = 0;
		}
		else
		{
			ptr = buf;
		}

		if (file == NULL)
		{
			merr_msg("file open error!");
			return;
		}
		if (totals <= 0 )
		{
			return;
		}

		//fseek(file, 0, SEEK_END);	/* 移到末尾 */
		if (totals <= rec_bytes)
		{
			append_bytes(file, ptr, totals);
			totals = 0;
		}
		else
		{
			append_bytes(file, ptr, rec_bytes);
			totals -= rec_bytes;
		}
		fflush(file);
	}
	fflush(file);
	shutdown(sock_fd, 0);
	fclose(file_tmp);
}

/* 加入新的数据 */
void append_bytes(FILE *file, const char *buf, const int size)
{
	fwrite(buf, 1, size, file);
}

void get_filename(const char *url, char *filename)
{
	const char *ptr = NULL;

	assert(url != NULL && filename != NULL);

	ptr = url + strlen(url);

	while (*(--ptr) != '/') {}

	strcpy(filename, ptr + 1);
	puts("filename:");
	puts(filename);
}
