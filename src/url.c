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
}

void add_new_url(RootUrl *root_url, const char *url)
{
	unsigned int url_id = get_url_id();
}

/* 添加新的Url */
Url * new_url_node(const char *url)
{
	Url *cur = NULL;

	assert(url != NULL);
	cur = (Url *) calloc (1, sizeof(Url));
	if (cur = NULL)
	{
		merr_msg("calloc error!");
		return;
	}
}

/* 获取取唯一的url_id */
static unsigned int get_url_id()
{
	if (count_url_id + 1 >= MAX_URL_ID)
	{
		merr_msg("count_url_id = %u, out_of_bound!", count_url_id);
	}

	/* protect the expression */
	M_LOCK(++count_url_id);

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
