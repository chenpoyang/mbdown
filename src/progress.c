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
	char str[4];
	char spd;
	int index, res, head, tail, unit, len;

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
	
	/* 计算速度值, 占八个字符,第一个字符为空格!, eg: | 29.1M/s| */
	while (res > unit)
	{
		res -= unit;
	}
	tail = res;
	head = got_bytes - tail;
	index = 0;
	ret_spd[index++] = ' ';
	if ('M' == spd || 'G' == spd)
	{
		/* head, eg: part of "29.1M/s" */
		m_utoa(head, str);
		len = strlen(str);
		strncpy(ret_spd + index, str, len);
		index += len;
		ret_spd[index++] = '.';

		/* tail, eg: part of "29.1M/s" */
		m_utoa(tail * 10 / unit, str);	/* 小数点后的整数 */
		len = strlen(str);
		strncpy(ret_spd + index, str, len);
		index += len;
		while (index < 5)
		{
			ret_spd[index++] = ' ';
		}
		ret_spd[indesx] = '\0';
	}
	else
	{
		m_utoa(head, str);
		len = strlen(str);
		strncpy(ret_spd + index, str, len);
		index += len;

		if (index <= 3)
		{
			ret_spd[index++] = '.';
			m_utoa(tail * 10 / unit, str);
			ret_spd[index++] = *str;
		}

		while (index < 5)
		{
			ret_spd[index++] = ' ';
		}
		ret_spd[index] = '\0';
	}

	ret_spd[index++] = spd;
	ret_spd[index++] = '/';
	ret_spd[index++] = 's';
	ret_spd[index] = '\0';
}

void get_rec_bytes(const int done, char *ret)
{
	char str[9], *ptr = NULL;	/* | ___,___,___| */
	int index, len;

	index = 0;
	ret[index++] = ' ';
	m_utoa(done, str);
	len = strlen(str);
	ptr = str;
	while (len > 3)
	{
		strncpy(ret + index, ptr, 3);
		index += 3;
		ret[index++] = ',';
		len -= 3;
		ptr += 3;
	}
	strncpy(ret + index, ptr, len);
	index += len;
	ret[index] = '\0';
}

/* get the bar: [------->        ] 
 * width = 4 + 1 + left + 1 + 12 + 8 + 14, default width: 85, left = 45!
 * width: 总的宽度
 * perc: 百分整, eg: the interger of 80%, 85%, 75% etc
 */
void get_ptr_bar(int perc, const int width, char *ret)
{
	int index, left, head, tail;

	index = 0;
	ret[index++] = '[';

	left = width - 4 - 1 - 1 - 12 - 8 - 14;
	head = perc * width / 100;
	tail = left - tail;
	while (--head > 0)
	{
		ret[index++] = '-';
	}
	ret[index++] = '>';

	while (--tail >= 0)
	{
		ret[index++] = ' ';
	}

	ret[index++] = ']';
	ret[index] = '\0';
}
/*
 *|<------14---->|
 *|  ------------|
 *|  in  --.--   |
 *|  in  01s     |
 *|  in  12m 05s |
 *|  eta 01d 16h |
 *|<------14---->|
 *显示状态, 如剩余时间, 完成任务的总花时等, 占14个字符, 包括前面两空格字符!
 */
void get_status(const int tm_stm, const int done, const int totals, char *ret)
{
	int index, h, m, s;
	char tmp_str[10];

	index = 0;
	ret[index++] = ' ';
	ret[index++] = ' ';
	if (done >= 0 && done != totals)
	{
		ret[index++] = 'i';
		ret[index++] = 'n';
		ret[index++] = ' ';
		ret[index++] = ' ';

		/* 时分秒 */
		s = tm_stm;
		h = s / 60 / 60;
		s -= h * 60 * 60;
		m = s / 60;
		s -= m * 60;
	}
	else if (done >= 0 && done < totals)
	{
		ret[index++] = 'e';
		ret[index++] = 't';
		ret[index++] = 'a';
		ret[index++] = ' ';
	}

	while (index < 14)
	{
		ret[index++] = ' ';
	}
	ret[index] = '\0';
}

void create_image(Progress *pro, const int width)
{
	char *p = NULL;
	char spd[8], rec_bytes[12], per[4];

	assert(pro != NULL && width > 0);

	p = pro->buf;
	get_spd(pro->end_sec - pro->beg_sec, pro->done, spd);
	get_percent(pro->done, pro->total, per);
	get_rec_bytes(pro->done, rec_bytes);
	get_ptr_bar(pro->done * 100 / pro->total, width, p + 4);
}

void display_image(Progress *pro)
{
	fprintf(stdout, "%s", "\r");
	fprintf(stdout, "%s", pro->buf);
}
