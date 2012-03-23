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

/* single linked list */
typedef struct url {
	struct url *next;	/* single linked list */
	char url[ULEN];		/* complete url */
	char scheme[SLEN];	/* protocal, eg: http */
	char host[HLEN];	/* remote host, eg: www.innlab.net */
	char res[LLEN];		/* the location of resource below the host */
	int port;
	int id;				/* unique id for quickly find */
}Url, RootUrl;

/* hash table to speed up find url */
typedef struct hash_url {
	struct hash_url *next;
	struct url *url;
}HaUrl;

/* initialise root_url for managing other urls */
void init_root_url(RootUrl *root_url);

/* release all resource */
void release_all(RootUrl *root_url);

/* return scheme of the url */
void get_scheme(const Url *url, char *scheme);

#endif //_URL_H_
