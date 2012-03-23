#include <stdlib.h>
#include <assert.h>

#include "bmsg.h"
#include "global.h"

/* describe breakpoints messages of all url */
RootBmsg *root_bmsg = NULL;

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

/* find url header */
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
	}

	return found ? cur : NULL;
}

/* add a new bmsg to specified url */
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

	bmsg->next = NULL;	/* make sure it is the last one of an url */
	cur->next = bmsg;	/* add to the tail */
}

/* initialise root_bmsg to manages breakpoints of all urls */
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

/* add a new breakpoints message to the queue */
void add_url_header(RootBmsg *root_bmsg, const Bmsg *header)
{
	int tar_url_id, found;
	Bmsg *cur = NULL;

	assert(root_bmsg != NULL);
	assert(header != NULL);

	tar_url_id = header->url_id;
	cur = root_bmsg->head;
	found = 0;	/* not found and the begining */
	while (cur)	/* find url header by tar_url_id */
	{
		if (cur->url_id == tar_url_id)
		{
			found = 1;
		}
		cur = cur->next_hdr;
	}

	if (found)	/* duplicate url header, logical error */
	{
		m_error("duplicate url header!");
		return;
	}

	/* add to the tail */
	if (root_bmsg->tail == NULL)
	{
		root_bmsg->head = root_bmsg->tail = (Bmsg *)header;
	}
	else
	{
		root_bmsg->tail->next_hdr = (Bmsg *)header;
		root_bmsg->tail = (Bmsg *)header;
	}

	root_bmsg->tail->next_hdr = NULL;	/* below the tail is NULL */
}
