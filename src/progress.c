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

/**
 * @brief	根据任务完成的份额, 计算百分额, 用ptr_buf[](占四字节)返回
 *
 * @param	done:	任务在总任务的完成额
 * @param	totals:	总任务份额
 * @param	ptr_buf: 返回数组
 */
void get_percent(const int done, const int totals, char *ptr_buf)
{
	int perc;

	assert(ptr_buf != NULL);

	if (totals <= 0 || done <= 0)
	{
		perc = 0;
	}
	else
	{
		perc = done * 100 / totals;
	}

	if (perc != 100)
	{
		if (perc < 10)
		{
			ptr_buf[0] = ' ';
			ptr_buf[1] = ' ';
			ptr_buf[2] = perc + '0';
			ptr_buf[3] = '%';
		}
		else
		{
			ptr_buf[0] = ' ';
			ptr_buf[1] = perc / 10;
			ptr_buf[2] = perc % 10;
			ptr_buf[3] = '%';
		}
	}
	else
	{
		ptr_buf[0] = '1';
		ptr_buf[1] = '0';
		ptr_buf[2] = '0';
		ptr_buf[3] = '%';
	}
	ptr_buf[4] = '\0';
}

/**
 * @brief	根据完成的任务量got_bytes及时间间隔, 计算速度
 *
 * @param	tm_stm: 任务开始到现在的时间间隔
 * @param	got_bytes: 完成的任务量
 * @param	ret_spd:	用字符串返回速度(占八个字符, 包括第一个空格字符!)
 */
void get_spd(const int tm_stm, const int got_bytes, char *ret_spd)
{
	/* B,K,M,G/s */
	char *ch = "BKMG";
	char spd;
	int index, res, head, tail, unit;

	assert(ret_spd != NULL);

	/* 确定速度单位 */
	index = 0;
	res = got_bytes;
	while(res > 1024 && index < 4)
	{
		++index;
		res /= 1024;
	}
	spd = ch[index];	/* 速度单位 */

	res = got_bytes;
	if (index == 0) /* B/s */
	{
		unit = 1;
	}
	else if (index == 1) /* K/s */
	{
		unit = 1024;
	}
	else if (index == 2)	/* M/s */
	{
		unit = 1024 * 1024;
	}
	else if (index == 3) /* G/s */
	{
		unit = 1024 * 1024 * 1024;
	}
	
	/* 计算速度值 */
	while (res > unit)
	{
		res -= unit;
	}
	tail = res;
	head = got_bytes - tail;
	index = 0;
	while (index < 4 && head > 0)
	{
	}
}

void create_image(Progress *pro, const int width)
{
	char *p = NULL;

	assert(pro != NULL && width > 0);

	p = pro->buf;
}

void display_image(Progress *pro)
{
	fprintf(stdout, "%s", "\r");
	fprintf(stdout, "%s", pro->buf);
}
