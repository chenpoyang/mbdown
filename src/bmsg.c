#include <stdlib.h>
#include <assert.h>

#include "bmsg.h"
#include "global.h"

/**
 * @brief	所有url的断点信息
 */
RootBmsg *root_bmsg = NULL;

/**
 * @brief	将一断点信息添加到根断点信息管理处
 *
 * @param	root_bmsg: 管理所有断点信息
 * @param	bmsg: 某一请求url的断点信息
 */
/* add a new bmsg to root_bmsg */
void add_bmsg(RootBmsg *root_bmsg, Bmsg *bmsg)
{
	Bmsg *url_header = NULL;

	assert(root_bmsg != NULL && bmsg != NULL);

	url_header = find_url_header(root_bmsg, bmsg->url_id);

	if (url_header == NULL)
	{
		/* added a header to root_bmsg */
		add_url_header(root_bmsg, url_header);
	}
	else
	{
		/* added a bmsg to an url */
		add_bmsg_header(url_header, bmsg);
	}
}

/**
 * @brief	根据某一url的id标识, 找出些url的头结点
 *
 * @param	root_bmsg: 查找源
 * @param	url_id: 某一url的唯一标识符
 *
 * @return	成功找到, 则返回些url的头部, 否则返加空
 */
Bmsg * find_url_header(const RootBmsg *root_bmsg, const int url_id)
{
	Bmsg *cur = NULL;
	int found;

	assert(root_bmsg != NULL);

	cur = root_bmsg->head;
	found = 0;
	while (cur)
	{
		if (cur->url_id == url_id)
		{
			found = 1;
			break;
		}
		cur = cur->next_hdr;
	}

	return found ? cur : NULL;
}

/**
 * @brief	为一个指定的url添加断点信息
 *
 * @param	header: 此断点信息所属的url
 * @param	bmsg: 要添加的断点信息
 */
void add_bmsg_header(Bmsg *header, Bmsg *bmsg)
{
	Bmsg *cur = NULL;

	assert(header != NULL && bmsg != NULL);

	if (bmsg->url_id != header->url_id)
	{
		m_error("mismatch of url_id!");
		return;
	}
	
	cur = header;
	while (cur->next != NULL)
	{
		cur = cur->next;
	}

	/* 新的断点信息添加到尾部, 确保尾结点的next没有断点信息 */
	bmsg->next = NULL;
	/* 添加到末尾 */
	cur->next = bmsg;
}

/**
 * @brief	初始化根断点信息管理器
 *
 * @param	root_bmsg: 断点信息管理器
 */
void init_root_bmsg(RootBmsg *root_bmsg)
{
	if (root_bmsg != NULL)
	{
		m_error("previours linked list exist!");
		return;
	}

	root_bmsg = (RootBmsg *) calloc (1, sizeof(RootBmsg));
	if (root_bmsg == NULL)
	{
		m_error("calloc() error");
		return;
	}
}

/**
 * @brief	一个新的url加入到下载队列, 为其添加相应的首部
 *
 * @param	root_bmsg: url断点信息管理器
 * @param	header: 新加入的url, 头部标识唯一的url
 */
void add_url_header(RootBmsg *root_bmsg, const Bmsg *header)
{
	int tar_url_id, found;
	Bmsg *cur = NULL;

	assert(root_bmsg != NULL);
	assert(header != NULL);

	tar_url_id = header->url_id;
	cur = root_bmsg->head;
	found = 0;	/* 开始时没找到 */
	while (cur)	/* 通过唯一的url标识符长url头部 */
	{
		if (cur->url_id == tar_url_id)
		{
			found = 1;
		}
		cur = cur->next_hdr;
	}

	if (found)	/* 重复的url头部,逻辑错误 */
	{
		m_error("duplicate url header!");
		return;
	}

	/* 添加到尾部 */
	if (root_bmsg->tail == NULL)
	{
		root_bmsg->head = root_bmsg->tail = (Bmsg *)header;
	}
	else
	{
		root_bmsg->tail->next_hdr = (Bmsg *)header;
		root_bmsg->tail = (Bmsg *)header;
	}

	/* 尾结点的next没有指向结点 */
	root_bmsg->tail->next_hdr = NULL;
}
