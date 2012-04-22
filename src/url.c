#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "global.h"
#include "synchro.h"
#include "url.h"

/* global variables */
RootUrl *root_url = NULL;				/* root url manager */
unsigned int count_url_id = 10;	/* for generating unique url id */


/* 初始化所有初始状态 */
void init_rooturl_id(RootUrl **root_url, unsigned int *url_id_beg)
{
	init_root_url(root_url);
	*url_id_beg = 0;
}

/* 初始化对象 */
void init_url_msg(Url *cur, const char *url)
{
	assert(url != NULL && url != NULL);

	/* url->id, 和url->next 已在 new_url_node 中初始化 */
	strcpy(cur->url, url);
	get_scheme(url, cur->scheme);
	get_host_name(url, cur->host);
	get_resource(url, cur->res);
	cur->port = get_port(url);
	cur->id = get_url_id();
}

/**
 * @brief	从root_url管理器中删除一url
 *
 * @param	root_url: Url管理器
 * @param	url: 要删除的url
 */
void remove_url(RootUrl *root_url, const Url *url)
{
	Url *cur = NULL, *tmp_ptr = NULL;

	if (root_url == NULL || url == NULL)
	{
		merr_msg("no need to remove url!");
		return;
	}

	M_LOCK_ROOTURL;
	cur = root_url;
	while (cur->next)
	{
		if (cur->next == url)
		{
			tmp_ptr = cur->next;
			cur->next = cur->next->next;
			free(tmp_ptr);
			tmp_ptr = NULL;
			break;
		}

		cur = cur->next;
	}
	M_UNLOCK_ROOTURL;
}

/* initialise root_url for managing other urls */
void init_root_url(RootUrl **root_url)
{
	M_LOCK_ROOTURL;
	*root_url = (RootUrl *) calloc (1, sizeof(RootUrl));
	M_UNLOCK_ROOTURL;
	if (*root_url == NULL)
	{
		merr_msg("calloc error!");
		return;
	}
}

/* 释放所有url结点 */
void release_root_url(RootUrl **root_url)
{
	Url *cur_ptr = NULL, *tmp_ptr = NULL;

	if (NULL == *root_url)
	{
		return;
	}

	M_LOCK_ROOTURL;
	cur_ptr = (*root_url);
	while (cur_ptr->next)
	{
		tmp_ptr = cur_ptr->next;

		cur_ptr->next = cur_ptr->next->next;

		free(tmp_ptr);
		tmp_ptr = NULL;
	}

	free(*root_url);
	*root_url = NULL;
	M_UNLOCK_ROOTURL;
}

/* 添加一条新的Url信息到root_url管理器中, 并哈希 */
void add_new_url(RootUrl **root_url, Url *url)
{
	assert(*root_url != NULL && url != NULL);

	M_LOCK_ROOTURL;

	/* 加到链表尾 */
	url->next = (*root_url)->next;
	(*root_url)->next = url;
	M_UNLOCK_ROOTURL;
}

/* 用完整url字符串,添加新的Url, 并加入链表中, 并哈希 */
Url * new_url_node(RootUrl **root_url, const char *url)
{
	Url *cur = NULL;

	assert(*root_url != NULL && url != NULL);
	cur = (Url *) calloc (1, sizeof(Url));
	if (cur == NULL)
	{
		merr_msg("calloc error!");
		return NULL;
	}

	init_url_msg(cur, url);

	/**
	 * @brief	将新的url结点放入root_url管理器中
	 *
	 * @param	root_url
	 * @param	url
	 */
	add_new_url(root_url, cur);

	return cur;
}

int get_port(const char *url)
{
	const char *ptr = NULL;
	int port = -1;

	assert(url != NULL);

	ptr = strstr(url, "//");
	if (ptr == NULL)
	{
		ptr = url;
		if (*ptr == '/')
		{
			ptr += 1;
		}
	}
	else
	{
		ptr += 2;
	}

	sscanf(ptr, "%*[^0-9]%d", &port);	/* 获取端口 */

	if (port == -1)	/* 没有显式指定端口, eg: http://www.innlab.net:8080/ */
	{
		port = 80;
	}


	return port;
}
/*
1. + URL 中+号表示空格 %2B 
2. 空格 URL中的空格可以用+号或者编码 %20 
3. / 分隔目录和子目录 %2F 
4. ? 分隔实际的 URL 和参数 %3F 
5. % 指定特殊字符 %25 
6. # 表示书签 %23 
7. & URL 中指定的参数间的分隔符 %26 
8. = URL 中指定参数的值 %3D
*/
/**
 * @brief	处理资源位置中特殊的字符
 *
 * @param	res: 源源的位置, eg: /index.html
 * @param	buf: 用于返回
 */
void escape_spec(const char *res, char *buf)
{
	int i, j, len, chg;
	char ch;

	assert(res != NULL && buf != NULL);

	len = strlen(res);
	for (i = j = 0; i < len; ++i)
	{
		switch(res[i])
		{
			case '+':
				buf[j++] = '%';
				buf[j++] = '2';
				buf[j++] = 'B';
				break;
			case ' ':
				buf[j++] = '%';
				buf[j++] = '2';
				buf[j++] = '0';
				break;
			case '/':
				buf[j++] = res[i];
				if (i + 1 < len && res[i + 1] == '/') 
				{
					++i;
					buf[j++] = '%';
					buf[j++] = '2';
					buf[j++] = 'F';
				}
				break;
			case '?':
				buf[j++] = '%';
				buf[j++] = '3';
				buf[j++] = 'F';
				break;
			case '%':
				chg = 0;	/* 不改为当前字符, eg: %2B */
				if (i + 2 < len && (res[i + 1] == '2' || res[i + 2] == '3'))
				{
					if (res[i + 1] == '2')
					{
						ch = res[i + 1];
						if (ch == 'B' || ch == 'b' || 
							ch == 'F' || ch == 'f' || 
							ch == '0' || ch == '5' || 
							ch == '3' || ch == '6')
						{
							chg = 0;
						}
						else
						{
							chg = 1;
						}
					}
					else
					{
						ch = res[i + 2];
						if (ch == 'F' || ch == 'f' || 
							ch == 'D' || ch == 'd')
						{
							chg = 0;
						}
						else
						{
							chg = 1;
						}
					}
				}
				else
				{
					chg = 1;
				}
				if (1 == chg)
				{
					buf[j++] = '%';
					buf[j++] = '2';
					buf[j++] = '5';
				}
				else
				{
					i += 2;
				}
				break;
			case '#':
				buf[j++] = '%';
				buf[j++] = '2';
				buf[j++] = '3';
				break;
			case '&':
				buf[j++] = '%';
				buf[j++] = '2';
				buf[j++] = '6';
				break;
			case '=':
				buf[j++] = '%';
				buf[j++] = '3';
				buf[j++] = 'D';
				break;
			default:
				buf[j++] = res[i];
				break;
		}
	}

	buf[j] = '\0';
}

/**
 * @brief	获取url字符串中指定的资源位置
 *
 * @param	url: url字符串
 * @param	resource: 用于返回
 */
void get_resource(const char *url, char *resource)
{
	const char *ptr = NULL, *tmp_ptr = NULL;

	assert(url != NULL && resource != NULL);

	ptr = strstr(url, "//");
	if (ptr == NULL)
	{
		ptr = url;
		if (*ptr == '/')
		{
			ptr += 1;
		}
	}
	else
	{
		ptr += 2;
	}

	tmp_ptr = strstr(ptr, "/");

	if (tmp_ptr == NULL)
	{
		merr_msg("invalid url!");
		return;
	}

	/* 处理特殊字符, 并用resource返回 */
	escape_spec(tmp_ptr, resource);
}
/* 获取取唯一的url_id, 已实现同步 */
unsigned int get_url_id()
{
	if (count_url_id + 1 >= MAX_URL_ID)
	{
		merr_msg("count_url_id = %u, out_of_bound!", count_url_id);
	}

	/* protect the expression */
	M_LOCK_ID;
	++count_url_id;
	M_UNLOCK_ID;

	return count_url_id - 1;
}

/**
 * @brief	根据给定的url地字符串, 返回协议
 *
 * @param	url: 字符串式的完整url
 * @param	scheme: 返回协议
 */
void get_scheme(const char *url, char *scheme)
{
	int len;
	char *tmp_ptr = NULL, *str = NULL;

	assert(url != NULL && scheme != NULL);

	str = strdup(url);
	if (str == NULL)
	{
		merr_msg("calloc error!");
		return;
	}

	strcpy(str, url);
	m_tolower(str);
	tmp_ptr = strstr(str, "//");

	if (tmp_ptr != NULL)
	{
		len = tmp_ptr - str;
		if (str[len - 0] == ':')
		{
			--len;
		}
		strncpy(scheme, str, len);	/* 返回协议 */
		scheme[len - 1] = '\0';
	}
	else
	{
		scheme[0] = '\0'; /* 返回协议 */
	}

	free(str);
	str = NULL;
}

/**
 * @brief	根据给定的url字符串, 返回主机的名字
 *
 * @param	url: 完整的url地址
 * @param	hostname: 用hostname返回
 */
void get_host_name(const char *url, char *hostname)
{
	char *ptr = NULL, *str_ptr = NULL, str[MAXLINE];
	int len;

	assert(url != NULL && hostname != NULL);

	len = strlen(url);

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
}

/* 输出Url里的值 */
void print_url(const Url *url)
{
	if (NULL == url)
	{
		merr_msg("url == NULL!");
		return;
	}
	printf("unique url id: %d\r\n", url->id);
	printf("complete url: %s\r\n", url->url);
	printf("scheme: %s\r\n", url->scheme);
	printf("host: %s\r\n", url->host);
	printf("res: %s\r\n", url->res);
	printf("port: %d\r\n", url->port);
}
