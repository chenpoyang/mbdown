#include <stdio.h>
#include <assert.h>
#include "progress.h"

void init_progress(Progress *pro, unsigned int total)
{
	char *p = NULL;
	int i;

	assert(pro != NULL);

	pro->done = 0;
	pro->total = total;
	pro->width = DEFAULT_WIDTH;
	pro->beg_sec = time(NULL);

	p = pro->buf;

	/* {"  0%"}-->4 ch! */ 
	*p++ = ' ';
	*p++ = '0';
	*p++ = '%';
	*p++ = ' ';

	/* {"[...]"}-->47 ch! */
	*p++ = '[';
	i = 44;
	*p++ = '>';
	while (--i >= 0)
	{
		*p++ = ' ';
	}
	*p++ = ']';

	/* {" ---,---,---"}-->12 ch! */
	*p++ = ' ';
	*p++ = '0';
	i = 10;
	while (--i >= 0)
	{
		*p++ = ' ';
	}

	/* {" -------"}-->8 ch!, defaule: " --.-K/s" */
	*p++ = ' ';
	*p++ = '-';
	*p++ = '-';
	*p++ = '.';
	*p++ = '-';
	*p++ = 'K';
	*p++ = '/';
	*p++ = 's';

	/* {"  ------------"}-->14 ch! */
	*p++ = ' ';
	*p++ = ' ';
	*p++ = 'e';
	*p++ = 't';
	*p++ = 'a';
	*p++ = ' ';
	*p++ = '-';
	*p++ = '-';
	*p++ = 'm';
	*p++ = ' ';
	*p++ = '-';
	*p++ = '-';
	*p++ = 's';
	*p++ = ' ';

	*p++ = '\0';
}

void update_progress(Progress *pro, int recv_buf)
{
	assert(pro != NULL && recv_buf > 0);

	pro->done += recv_buf;
	pro->end_sec = time(NULL);

	create_image(pro, DEFAULT_WIDTH);	/* 默认宽度: 85 */
	display_image(pro);
}

void create_image(Progress *pro, const int width)
{
	char *p = NULL;

	assert(pro != NULL && width > 0);
}

void display_image(Progress *pro)
{
	fprintf(stdout, "%s", "\r");
	fprintf(stdout, "%s", pro->buf);
}
