#ifndef _BMSG_H_
#define _BMSG_H_

/* 断点的单元 */
#define UNIT (sizeof(int))

/**
 * @brief   保存断点信息的结构(单个url), [begin, end)
 */
typedef struct bmsg {
    struct bmsg *next;          /* 单链表 */
    struct bmsg *next_hdr;      /* 下一个结点 */
    int url_id;                 /* 唯一标识url结点 */
    unsigned int begin;         /* 断点开始位置 */
    unsigned int end;           /* 断点结束位置 */
    int to_bytes;               /* 总大小 */
    int to_bps;                 /* 断点数 */
}Bmsg;


/**
 * @brief   管理理所有的下载任务
 */
typedef struct root_bmsg {
    struct bmsg *head;
    struct bmsg *tail;
}RootBmsg;

/* 初始化url断点管理器, 管理所有下载任务的所有断点信息 */
void init_root_bmsg(RootBmsg *root_bmsg);

/* 将新的断点信息添加到指定的url断点组中 */
void add_bmsg_header(Bmsg *header, Bmsg *bmsg);
/* 将一个url断点单元添加到管理器中 */
void add_url_header(RootBmsg *root_bmsg, const Bmsg *header);
/* 将一断点信息添加到url断点管理器中 */
void add_bmsg(RootBmsg *root_bmsg, Bmsg *bmsg);
/* 找url的头部 */
Bmsg * find_url_header(const RootBmsg *root_bmsg, const int url_id);

#endif // _BMSG_H_
