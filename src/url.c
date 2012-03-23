#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "global.h"
#include "url.h"

/* root url manager */
RootUrl *root_url = NULL;

/* hash table to speed up find url */
HaUrl ha_url[HALEN];

/* initialise root_url for managing other urls */
void init_root_url(RootUrl *root_url)
{
	if (root_url != NULL)
	{
		m_error("previous url linked list exist!");
		return;
	}
	
	root_url = (RootUrl *) calloc (1, sizeof(RootUrl));
	if (root_url == NULL)
	{
		m_error("calloc error!");
		return;
	}
}

/* release all resource */
void release_all(RootUrl *root_url)
{
}

/* return scheme of the url */
void get_scheme(const Url *url, char *scheme)
{
}
