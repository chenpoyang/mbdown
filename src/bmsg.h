#ifndef _BMSG_H_
#define _BMSG_H_

/* the unit of fragement */
#define UNIT (sizeof(int))

/* structure to describe the breakboint message,
 * start from [begin*UNIT, end*UNIT) */
typedef struct bmsg {
	struct bmsg *next;		/* single linked list */
	struct bmsg *next_hdr;	/* netx url bmsgs */
	int url_id;
	unsigned short begin;
	unsigned short end;
	int to_bytes;
	int to_bps;		/* the numbsers of breakpoints */
}Bmsg;

/* queue for mannagering all urls' bmsgs */
typedef struct root_bmsg {
	struct bmsg *head;
	struct bmsg *tail;
}RootBmsg;

/**
 * @brief	所有url的断点信息
 */
RootBmsg *root_bmsg = NULL;

/* initialise root_bmsg to manages breakpoints of all urls */
void init_root_bmsg(RootBmsg *root_bmsg);

/* add a new bmsg to specified url */
void add_bmsg_header(Bmsg *header, Bmsg *bmsg);
/* add a new url header to the queue */
void add_url_header(RootBmsg *root_bmsg, const Bmsg *header);
/* add a new bmsg to root_bmsg */
void add_bmsg(RootBmsg *root_bmsg, Bmsg *bmsg);
/* find url header */
Bmsg * find_url_header(const RootBmsg *root_bmsg, const int url_id);

#endif // _BMSG_H_
