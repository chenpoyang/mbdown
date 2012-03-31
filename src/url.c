#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "global.h"
#include "synchro.h"
#include "url.h"

/* global variables */
RootUrl *root_url = NULL;	/* root url manager */
static unsigned int count_url_id = 0;	/* for generating unique url id */
HaUrl url_htable[HALEN];


/* 初始化所有初始状态 */
void init_all()
{
	init_hash_table(url_htable, HALEN);
	init_root_url(root_url);
}

/**
 * @brief	在长度为len的哈希表url_htable中查找标识为url_id的url结点
 *
 * @param	url_htable
 * @param	len
 * @param	url_id
 *
 * @return	若找到, 返加Url结点的地址, 否则返回NULL
 */
Url * find_url(HaUrl *url_htable, const int len, const int url_id)
{
	HaUrl *cur = NULL;

	assert(url_htable != NULL);

	cur = url_htable + (url_id % HALEN);
	while (cur->next)
	{
		if (cur->next->url->id == url_id)
		{
			return cur->next->url;
		}
		cur = cur->next;
	}

	return NULL;
}

/**
 * @brief	从root_url管理器中删除一url
 *
 * @param	root_url: Url管理器
 * @param	url: 要删除的url
 */
void remove_url(RootUrl *root_url, const Url *url)
{
}

/**
 * @brief	为url建立哈希索引
 *
 * @param	url_htable: 哈希表
 * @param	url: url结点
 */
void hash_url(HaUrl *url_htable, Url *url)
{
	int index;
	HaUrl *cur = NULL;
	
	assert(url_htable != NULL && url != NULL);

	index = url->id % HALEN;	/* 找索引 */
	cur = url_htable + index;	/* 找到相应的链表 */
	while (cur->next)
	{
		cur = cur->next;
	}

	/* cur->next所有成原置空 */
	cur->next = (HaUrl *) calloc (1, sizeof(HaUrl));
	if (NULL == cur->next)
	{
		merr_sys("calloc error!");
		return;
	}

	/* 追加url */
	cur->next->url = url;
}

/**
 * @brief	将url结点比url_htable中删除
 *
 * @param	url_htable
 * @param	url
 */
void unhash_url(HaUrl *url_htable, Url *url)
{
	HaUrl *cur = NULL, *tmp_ptr = NULL;

	assert(url_htable != NULL && url != NULL);

	cur = &url_htable[url->id % HALEN];
	while (cur->next)
	{
		if (cur->next->url == url)
		{
			tmp_ptr = cur->next;
			cur->next = cur->next->next;
			free(tmp_ptr);

			tmp_ptr = NULL;
			return;
		}
		cur = cur->next;
	}

}

/**
 * @brief	初始化哈希表:url_hashtable[], 表长度为len
 *
 * @param	url_htable:哈希表首地址
 * @param	len:	表长度
 */
void init_hash_table(HaUrl *url_htable, const int len)
{
	int i;

	i = len;
	while (--i >= 0)
	{
		url_htable[i].next = NULL;
		url_htable[i].url = NULL;
	}
}

/* initialise root_url for managing other urls */
void init_root_url(RootUrl *root_url)
{
	if (root_url != NULL)
	{
		merr_msg("previous url linked list exist!");
		return;
	}
	
	root_url = (RootUrl *) calloc (1, sizeof(RootUrl));
	if (root_url == NULL)
	{
		merr_msg("calloc error!");
		return;
	}
}

/* release all resource */
void release_all(RootUrl *root_url)
{
	Url *cur_ptr = NULL, *tmp_ptr = NULL;

	if (NULL == root_url)
	{
		return;
	}

	cur_ptr = root_url->next;
	while (cur_ptr)
	{
		tmp_ptr = cur_ptr;

		cur_ptr = cur_ptr->next;

		free(tmp_ptr);
		tmp_ptr = NULL;
	}

	free(root_url);
	root_url = NULL;
}

void add_new_url(RootUrl *root_url, const char *url)
{
	Url *new_url = NULL;

	assert(root_url != NULL && url != NULL);

	new_url = new_url_node(url);
}

/* 用完整url字符串,添加新的Url */
Url * new_url_node(const char *url)
{
	Url *cur = NULL;

	assert(url != NULL);
	cur = (Url *) calloc (1, sizeof(Url));
	if (cur == NULL)
	{
		merr_msg("calloc error!");
		return NULL;
	}

	strcpy(cur->url, url);
	get_scheme(url, cur->scheme);
	get_host_name(url, cur->host);
	get_resource(url, cur->res);
	cur->port = get_port(url);
	cur->id = get_url_id();

	return cur;
}


int get_port(const char *url)
{
	char *ptr = NULL;
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

/**
 * @brief	获取url字符串中指定的资源位置
 *
 * @param	url: url字符串
 * @param	resource: 用于返回
 */
void get_resource(const char *url, char *resource)
{
	char *ptr = NULL, *tmp_ptr = NULL;

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

	strcpy(resource, tmp_ptr);
}
/* 获取取唯一的url_id */
unsigned int get_url_id()
{
	if (count_url_id + 1 >= MAX_URL_ID)
	{
		merr_msg("count_url_id = %u, out_of_bound!", count_url_id);
	}

	/* protect the expression */
	M_LOCK;
	++count_url_id;
	M_UNLOCK;

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

	str = (char *) calloc (1, sizeof(url));
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
