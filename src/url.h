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

/* hash table to speed up find url */
typedef struct hash_url {
	struct hash_url *next;
	struct url *url;
}HaUrl;

/* 初始化所有初始状态 */
void init_hatable_rooturl_id();

/* 初始化对象 */
void init_url_msg(Url *url, const char *url_str);

/* 初始化根Url结点, 方便管理其他Url */
void init_root_url(RootUrl *root_url);

/* 获取取唯一的url_id, 已实现同步 */
unsigned int get_url_id();

/* 添加一条新的Url信息到root_url管理器中, 并哈希 */
void add_new_url(RootUrl *root_url, Url *url, HaUrl *ha_table, const int ha_len);

/* 用完整url字符串,添加新的Url, 并加入链表中, 并哈希 */
Url * new_url_node(RootUrl *root_url, const char *url, HaUrl *ha_table, const int ha_len);

/* 释放所有url结点资源 */
void release_root_url(RootUrl *root_url);

/* 释放哈希表资源 */
void release_hatable(HaUrl *hatable, const int len);

/* 清理所有资源 */
void release_url_all(RootUrl *root_url, HaUrl *hatable, const int len);

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
 * @brief	初始化哈希表:url_hashtable[], 表长度为len
 *
 * @param	url_htable:哈希表首地址
 * @param	len:	表长度
 */
void init_hash_table(HaUrl *url_htable, const int len);

/**
 * @brief	为url建立哈希索引
 *
 * @param	url_htable: 哈希表
 * @param	url: url结点
 */
void hash_url(HaUrl *url_htable, Url *url, const int ha_len);

/**
 * @brief	将url结点比url_htable中删除
 *
 * @param	url_htable
 * @param	url
 */
void unhash_url(HaUrl *url_htable, Url *url);

/**
 * @brief	在长度为len的哈希表url_htable中查找标识为url_id的url结点
 *
 * @param	url_htable
 * @param	len
 * @param	url_id
 *
 * @return	若找到, 返加Url结点的地址, 否则返回NULL
 */
Url * find_url(HaUrl *url_htable, const int len, const int url_id);

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

#endif //_URL_H_
