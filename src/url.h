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
/* 最大url_id标识符 */
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

/* 初始化所有初始状态 */
void init_rooturl_id(RootUrl **root_url, unsigned int *url_id_beg);

/* 初始化对象 */
void init_url_msg(Url *url, const char *url_str);

/* 初始化根Url结点, 方便管理其他Url */
void init_root_url(RootUrl **root_url);

/* 获取取唯一的url_id, 已实现同步 */
unsigned int get_url_id();

/* 添加一条新的Url信息到root_url管理器中, 并哈希 */
void add_new_url(RootUrl **root_url, Url *url);

/* 用完整url字符串,添加新的Url, 并加入链表中, 并哈希 */
Url * new_url_node(RootUrl **root_url, const char *url);

/* 释放所有url结点资源 */
void release_root_url(RootUrl **root_url);

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

int get_port(const char *url);

/**
 * @brief	获取url字符串中指定的资源位置
 *
 * @param	url: url字符串
 * @param	resource: 用于返回
 */
void get_resource(const char *url, char *resource);

/**
 * @brief	从root_url管理器中删除一url
 *
 * @param	root_url: Url管理器
 * @param	url: 要删除的url
 */
void remove_url(RootUrl *root_url, const Url *url);

/**
 * @brief	处理资源位置中特殊的字符
 *
 * @param	res: 源源的位置, eg: /index.html
 * @param	ret_str: 用于返回
 */
void escape_spec(const char *res, char *ret_str);

/* 输出Url里的值 */
void print_url(const Url *url);

/* 从url中获取域名并解释成ip地址, 由ret_ip返回ip */
void getip(const char *url, char *ret_ip);

#endif //_URL_H_
