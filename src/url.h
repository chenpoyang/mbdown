#ifndef _URL_H_
#define _URL_H_

/* define the max length of url */
#define ULEN	256
/* the max length of host, eg: www.innlab.net */
#define	HLEN	32
/* the max length of location */
#define LLEN	(ULEN-HLEN)
/* the max length of scheme */
#define SLEN	24
/* 最的url_id标识符 */
#define MAX_URL_ID 0xffffffff

/* single linked list */
typedef struct url {
	struct url *next;	/* single linked list */
	char url[ULEN];		/* complete url */
	char scheme[SLEN];	/* protocal, eg: http */
	char host[HLEN];	/* remote host, eg: www.innlab.net */
	char res[LLEN];		/* the location of resource below the host */
	int port;
	unsigned int id;				/* unique id for quickly find */
}Url, RootUrl;

/* hash table to speed up find url */
typedef struct hash_url {
	struct hash_url *next;
	struct url *url;
}HaUrl;

/* 初始化根Url结点, 方便管理其他Url */
void init_root_url(RootUrl *root_url);

/* 获取取唯一的url_id */
static unsigned int get_url_id();

/* 添加一条新的Url信息 */
void add_new_url(RootUrl *root_url, const char *url);

/* 添加新的Url */
Url * new_url_node(const char *url);

/* release all resource */
void release_all(RootUrl *root_url);

/**
 * @brief	根据给定的url地字符串, 返回协议
 *
 * @param	url: 字符串式的完整url
 * @param	scheme: 返回协议
 */
void get_scheme(const char *url, char *scheme);

/**
 * @brief	根据给定的url字符串, 返回主机的名字
 *
 * @param	url: 完整的url地址
 * @param	hostname: 用hostname返回
 */
void get_host_name(const char *url, char *hostname);

#endif //_URL_H_
